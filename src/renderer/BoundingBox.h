#pragma once


#include <vector>

class BoundingBox {
public:
    BoundingBox(glm::vec4 min, glm::vec4 max);
    glm::vec4 min;
    glm::vec4 max;
};