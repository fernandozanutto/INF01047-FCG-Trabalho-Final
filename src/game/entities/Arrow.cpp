#include "Arrow.h"
#include "../../renderer/Model.h"

#include <glm/mat4x4.hpp>

Model* arrowModel;


Model* start() {
    if (arrowModel == NULL) {
        arrowModel = new Model("arrow");
    }

    return arrowModel;
}

Arrow::Arrow() : GameObject(start()) {
    modelOffset = glm::vec4(1, 0, 0, 0);
}