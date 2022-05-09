#pragma once

#include <vector>
#include <glm/vec4.hpp>

class BoundingBox {
public:
    BoundingBox(glm::vec4 min, glm::vec4 max);
    glm::vec4 min;
    glm::vec4 max;
};