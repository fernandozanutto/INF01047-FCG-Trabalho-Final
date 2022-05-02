#include <glm/mat4x4.hpp>

#include "Arrow.h"
#include "../../renderer/Model.h"

Model* arrowModel;

Model* getArrowModel() {
    if (arrowModel == NULL) {
        arrowModel = new Model("arrow");
    }

    return arrowModel;
}

Arrow::Arrow() : GameObject(getArrowModel()) {
    modelOffset = glm::vec4(-scaleVector.x, 0, 0, 0);
    collisionType = Point;
}