#include <iostream>

#include <tiny_obj_loader.h>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include "Matrices.h"
#include "Model.h"

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