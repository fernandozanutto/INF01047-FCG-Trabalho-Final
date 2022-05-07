#include <iostream>

#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Matrices.h"
#include "Model.h"

Model::Model(std::string filename) : name(filename) {
    renderType = Model::FROM_MODEL;
    loadModel();
    createDebugModel();
    loadTexture();
}

Model::Model(std::string filename, RenderProjectType type) : name(filename), renderType(type) {
    loadModel();
    createDebugModel();
    loadTexture();
}

void Model::loadModel() {
    std::string modelLocation = "data/" + name + ".obj";

    printf("Carregando modelo \"%s\"... ", name.c_str());

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelLocation.c_str(), NULL, true);

    if (!err.empty()) {
        fprintf(stderr, "\n%s\n", err.c_str());
    }

    if (!ret) {
        throw std::runtime_error("Erro ao carregar modelo.");
    }

    printf("OK. ");

    computeNormals();
    printf("compute normals OK. ");
    buildTriangles();
    printf("build triangles OK.\n");
}

unsigned char* findPossiblesTextures(std::string file, int* width, int* height, int* channels) {
    std::vector<std::string> possibleExtensions = {"png", "jpg", "jpeg"};

    unsigned char* data;
    
    for (std::string extension: possibleExtensions) {
        std::string textureFile = "data/" + file + "." + extension;
        printf("Procurando textura \"%s\"... ", textureFile.c_str());
        data = stbi_load(textureFile.c_str(), width, height, channels, 3);
        if (data != NULL) break;
    }
    
    return data;
}

void Model::loadTexture() {
    stbi_set_flip_vertically_on_load(true);
    
    int width;
    int height;
    int channels;

    unsigned char* data = findPossiblesTextures(name, &width, &height, &channels);
    
    if (data == NULL) {
        std::cout << "Não foi possível carregar. Usando textura padrão.\n";
        data = stbi_load("data/missing_textures.png", &width, &height, &channels, 3);
    }

    std::cout << "OK (" << width << "x"<< height << ").\n";
    
    // Agora criamos objetos na GPU com OpenGL para armazenar a textura
    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    std::cout << "ID gerado: " << texture_id << std::endl;
    this->textureId = texture_id;
    glGenSamplers(1, &sampler_id);
    
    // Veja slides 95-96 do documento Aula_20_Mapeamento_de_Texturas.pdf
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Parâmetros de amostragem da textura.
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Agora enviamos a imagem lida do disco para a GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Model::computeNormals() {
    if (!attrib.normals.empty())
        return;

    std::cout << "computing normals... ";

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

void Model::buildTriangles() {
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);
    this->vaoId = vertex_array_object_id;
    std::vector<GLuint> indices;
    std::vector<float> model_coefficients;
    std::vector<float> normal_coefficients;
    std::vector<float> texture_coefficients;

    for (size_t shape = 0; shape < shapes.size(); ++shape) {
        size_t first_index = indices.size();
        size_t num_triangles = shapes[shape].mesh.num_face_vertices.size();

        const float minval = std::numeric_limits<float>::min();
        const float maxval = std::numeric_limits<float>::max();

        glm::vec4 bbox_min = glm::vec4(maxval, maxval, maxval, 1);
        glm::vec4 bbox_max = glm::vec4(minval, minval, minval, 1);

        for (size_t triangle = 0; triangle < num_triangles; ++triangle) {
            assert(shapes[shape].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex) {
                tinyobj::index_t idx = shapes[shape].mesh.indices[3 * triangle + vertex];

                indices.push_back(first_index + 3 * triangle + vertex);

                const float vx = attrib.vertices[3 * idx.vertex_index + 0];
                const float vy = attrib.vertices[3 * idx.vertex_index + 1];
                const float vz = attrib.vertices[3 * idx.vertex_index + 2];

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

        size_t last_index = indices.size();

        shapeName.push_back(shapes[shape].name);
        firstIndex.push_back(first_index);
        numIndexes.push_back(last_index - first_index);
        this->boundingBoxes.push_back(BoundingBox(bbox_min, bbox_max));
        renderingMode.push_back(GL_TRIANGLES);
    }

    createVBObject(model_coefficients.data(), model_coefficients.size() * sizeof(float), 0, 4);

    if (!normal_coefficients.empty()) {
        createVBObject(normal_coefficients.data(), normal_coefficients.size() * sizeof(float), 1, 4);
    }

    if (!texture_coefficients.empty()) {
        createVBObject(texture_coefficients.data(), texture_coefficients.size() * sizeof(float), 2, 2);
    }

    createIBObject(indices.data(), indices.size() * sizeof(GLuint));

    glBindVertexArray(0);
}

void Model::createDebugModel() {
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);
    this->vaoDebugId = vertex_array_object_id;
    
    std::vector<GLuint> indices = {
    // Definimos os índices dos vértices que definem as linhas dos eixos X, Y,
    // Z, que serão desenhados com o modo GL_LINES.
        0, 1, // linha 1
        2, 3, // linha 2
        4, 5  // linha 3
    };

    std::vector<float> model_coefficients = {
    // Vértices para desenhar o eixo X
    //    X      Y     Z     W
         0.0f,  0.0f,  0.0f, 1.0f, // posição do vértice 8
         2.0f,  0.0f,  0.0f, 1.0f, // posição do vértice 9
    // Vértices para desenhar o eixo Y
    //    X      Y     Z     W
         0.0f,  0.0f,  0.0f, 1.0f, // posição do vértice 10
         0.0f,  2.0f,  0.0f, 1.0f, // posição do vértice 11
    // Vértices para desenhar o eixo Z
    //    X      Y     Z     W
         0.0f,  0.0f,  0.0f, 1.0f, // posição do vértice 12
         0.0f,  0.0f,  2.0f, 1.0f, // posição do vértice 13
    };

    std::vector<float> colors = {
    // Cores para desenhar o eixo X (vermelho)
        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 0
        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 1
    // Cores para desenhar o eixo Y (verde)
        0.0f, 1.0f, 0.0f, 1.0f, // cor do vértice 2
        0.0f, 1.0f, 0.0f, 1.0f, // cor do vértice 3
    // Cores para desenhar o eixo Z (azul)
        0.0f, 0.0f, 1.0f, 1.0f, // cor do vértice 4
        0.0f, 0.0f, 1.0f, 1.0f, // cor do vértice 5
    };
    
    debugFirstIndex = 0;
    debugNumIndexes = 6;
    debugRenderingMode = GL_LINES;

    createVBObject(model_coefficients.data(), model_coefficients.size() * sizeof(float), 0, 4);
    createVBObject(colors.data(), colors.size() * sizeof(float), 3, 4);
    createIBObject(indices.data(), indices.size() * sizeof(GLuint));

    glBindVertexArray(0);
}

void Model::createVBObject(const void* data, unsigned int size, int location, int dimensions) {
    unsigned int bufferId;

    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId); 
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::createIBObject(const void* data, unsigned int size) {
    unsigned int indexId;

    glGenBuffers(1, &indexId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId); 

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}