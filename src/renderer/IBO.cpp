#include "IBO.h"

#include <glad/glad.h>


IBO::IBO(const void* data, unsigned int size) {
    glGenBuffers(1, &indexId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId); 

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}