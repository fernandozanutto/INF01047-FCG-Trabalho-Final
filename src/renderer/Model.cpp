#include <iostream>

#include <tiny_obj_loader.h>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include "Matrices.h"
#include "Model.h"
#include "VBO.h"
#include "IBO.h"

Model::Model() { }

Model::Model(std::string filename) : name(filename) {

    std::string fileLocation = "data/" + filename + ".obj";

    printf("Carregando modelo \"%s\"... ", filename.c_str());

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileLocation.c_str(), NULL, true);

    if (!err.empty()) {
        fprintf(stderr, "\n%s\n", err.c_str());
    }

    if (!ret) {
        throw std::runtime_error("Erro ao carregar modelo.");
    }

    printf("OK.\n");

    computeNormals();
    buildTrianglesAndAddToVirtualScene();
}

void Model::computeNormals() {
    if (!attrib.normals.empty())
        return;

    std::cout << "Computando normais de " << name << std::endl;
    // Primeiro computamos as normais para todos os TRIÂNGULOS.
    // Segundo, computamos as normais dos VÉRTICES através do método proposto
    // por Gouraud, onde a normal de cada vértice vai ser a média das normais de
    // todas as faces que compartilham este vértice.

    size_t num_vertices = attrib.vertices.size() / 3;

    std::vector<int> num_triangles_per_vertex(num_vertices, 0);
    std::vector<glm::vec4> vertex_normals(num_vertices, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    for (size_t shape = 0; shape < shapes.size(); ++shape) {
        size_t num_triangles = shapes[shape].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle) {
            assert(shapes[shape].mesh.num_face_vertices[triangle] == 3);

            glm::vec4 vertices[3];
            for (size_t vertex = 0; vertex < 3; ++vertex) {
                tinyobj::index_t idx = shapes[shape].mesh.indices[3 * triangle + vertex];
                const float vx = attrib.vertices[3 * idx.vertex_index + 0];
                const float vy = attrib.vertices[3 * idx.vertex_index + 1];
                const float vz = attrib.vertices[3 * idx.vertex_index + 2];
                vertices[vertex] = glm::vec4(vx, vy, vz, 1.0);
            }

            const glm::vec4 a = vertices[0];
            const glm::vec4 b = vertices[1];
            const glm::vec4 c = vertices[2];

            const glm::vec4 n = crossproduct(b - a, c - a);

            for (size_t vertex = 0; vertex < 3; ++vertex) {
                tinyobj::index_t idx = shapes[shape].mesh.indices[3 * triangle + vertex];
                num_triangles_per_vertex[idx.vertex_index] += 1;
                vertex_normals[idx.vertex_index] += n;
                shapes[shape].mesh.indices[3 * triangle + vertex].normal_index = idx.vertex_index;
            }
        }
    }

    attrib.normals.resize(3 * num_vertices);

    for (size_t i = 0; i < vertex_normals.size(); ++i) {
        glm::vec4 n = vertex_normals[i] / (float)num_triangles_per_vertex[i];
        n /= norm(n);
        attrib.normals[3 * i + 0] = n.x;
        attrib.normals[3 * i + 1] = n.y;
        attrib.normals[3 * i + 2] = n.z;
    }
}

void Model::buildTrianglesAndAddToVirtualScene() {
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);

    std::vector<GLuint> indices;
    std::vector<float> model_coefficients;
    std::vector<float> normal_coefficients;
    std::vector<float> texture_coefficients;

    for (size_t shape = 0; shape < shapes.size(); ++shape) {
        size_t first_index = indices.size();
        size_t num_triangles = shapes[shape].mesh.num_face_vertices.size();

        const float minval = std::numeric_limits<float>::min();
        const float maxval = std::numeric_limits<float>::max();

        glm::vec3 bbox_min = glm::vec3(maxval, maxval, maxval);
        glm::vec3 bbox_max = glm::vec3(minval, minval, minval);

        for (size_t triangle = 0; triangle < num_triangles; ++triangle) {
            assert(shapes[shape].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex) {
                tinyobj::index_t idx = shapes[shape].mesh.indices[3 * triangle + vertex];

                indices.push_back(first_index + 3 * triangle + vertex);

                const float vx = attrib.vertices[3 * idx.vertex_index + 0];
                const float vy = attrib.vertices[3 * idx.vertex_index + 1];
                const float vz = attrib.vertices[3 * idx.vertex_index + 2];
                // printf("tri %d vert %d = (%.2f, %.2f, %.2f)\n", (int)triangle, (int)vertex, vx, vy, vz);
                model_coefficients.push_back(vx);    // X
                model_coefficients.push_back(vy);    // Y
                model_coefficients.push_back(vz);    // Z
                model_coefficients.push_back(1.0f);  // W

                bbox_min.x = std::min(bbox_min.x, vx);
                bbox_min.y = std::min(bbox_min.y, vy);
                bbox_min.z = std::min(bbox_min.z, vz);
                bbox_max.x = std::max(bbox_max.x, vx);
                bbox_max.y = std::max(bbox_max.y, vy);
                bbox_max.z = std::max(bbox_max.z, vz);

                if (idx.normal_index != -1) {
                    const float nx = attrib.normals[3 * idx.normal_index + 0];
                    const float ny = attrib.normals[3 * idx.normal_index + 1];
                    const float nz = attrib.normals[3 * idx.normal_index + 2];
                    normal_coefficients.push_back(nx);    // X
                    normal_coefficients.push_back(ny);    // Y
                    normal_coefficients.push_back(nz);    // Z
                    normal_coefficients.push_back(0.0f);  // W
                }

                if (idx.texcoord_index != -1) {
                    const float u = attrib.texcoords[2 * idx.texcoord_index + 0];
                    const float v = attrib.texcoords[2 * idx.texcoord_index + 1];
                    texture_coefficients.push_back(u);
                    texture_coefficients.push_back(v);
                }
            }
        }

        size_t last_index = indices.size() - 1;

        shapeName.push_back(shapes[shape].name);
        std::cout << "build triangles for " << shapes[shape].name << " " << &(vaoId) << std::endl;
        firstIndex.push_back(first_index);
        numIndexes.push_back(last_index - first_index + 1);
        vaoId.push_back(vertex_array_object_id);
        this->bbox_max.push_back(bbox_max);
        this->bbox_min.push_back(bbox_min);
        renderingMode.push_back(GL_TRIANGLES);
    }

    VBO VBO_model_coefficients(model_coefficients.data(), model_coefficients.size() * sizeof(float), 0, 4);

    if (!normal_coefficients.empty()) {
        VBO VBO_normal_coefficients(normal_coefficients.data(), normal_coefficients.size() * sizeof(float), 1, 4);
    }

    if (!texture_coefficients.empty()) {
        VBO VBO_texture_coefficients(texture_coefficients.data(), texture_coefficients.size() * sizeof(float), 2, 2);
    }

    IBO indexBuffer(indices.data(), indices.size() * sizeof(GLuint));

    glBindVertexArray(0);
}