#include <glm/mat4x4.hpp>

#include "Arrow.h"
#include "../../renderer/Model.h"
#define PI 3.1415926f

Model* arrowModel;

Model* getArrowModel() {
    if (arrowModel == NULL) {
        arrowModel = new Model("arrow");
    }

    return arrowModel;
}

Arrow::Arrow() : GameObject(getArrowModel()) {
    modelTranslateOffset = glm::vec4(-scaleVector.x, 0, 0, 0);
    modelRotationOffset = glm::vec4(0,-PI/2,0,0);
    collisionType = Point;
}