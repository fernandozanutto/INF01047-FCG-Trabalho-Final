#include <glm/mat4x4.hpp>

#include "Target.h"
#include "../../renderer/Model.h"

Model* targetModel;

Model* getTargetModel() {
    if (targetModel == NULL) {
        targetModel = new Model("target", Model::PLANAR);
    }

    return targetModel;
}

Target::Target() : GameObject(getTargetModel()) {
    modelTranslateOffset = glm::vec4(-scaleVector.x, 0, 0, 0);
    collisionType = OBB;
}