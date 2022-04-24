#include <glm/vec4.hpp>

#include "BoundingBox.h"


BoundingBox::BoundingBox(glm::vec4 min, glm::vec4 max) {
    this->max = max;
    this->min = min;
}