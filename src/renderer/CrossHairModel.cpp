#include "CrossHairModel.h"

#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <glad/glad.h>

CrossHairModel::CrossHairModel() {
    this->name = "CrossHair";

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    float vertices[] = {
        -.5f, 0.0f,  
         .5f, 0.0f,  
         0.0f,  .5f,  
         0.0f, -.5f
    };

    std::vector<float> colors = {
        0.0f, 1.0f, 0.0f, 1.0f, // cor do vértice 0
        0.0f, 1.0f, 0.0f, 1.0f, // cor do vértice 1
        0.0f, 1.0f, 0.0f, 1.0f, // cor do vértice 2
        0.0f, 1.0f, 0.0f, 1.0f, // cor do vértice 3
    };

    unsigned int indices[] = { 0, 1, 2, 3 };
    createVBObject(vertices, 8*sizeof(float), 0, 2);
    createVBObject(colors.data(),  colors.size()*sizeof(float), 1, 4);
    createIBObject(indices, 4*sizeof(int));

    this->vaoId = VAO;

    shapeName.push_back(name);
    firstIndex.push_back(0);
    numIndexes.push_back(4);
    renderingMode.push_back(GL_LINES);

    glBindVertexArray(0);
}
    
