#include "VBO.h"

#include <glad/glad.h>


VBO::VBO(const void* data, unsigned int size, int location, int dimensions) {
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId); 
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}