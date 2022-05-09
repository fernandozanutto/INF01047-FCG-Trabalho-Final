#include <iostream>
#include <time.h>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

#include "Arrow.h"
#include "../../renderer/Model.h"
#include "../../renderer/Matrices.h"

#define PI 3.1415926f

Model* arrowModel;

Model* getArrowModel() {
    if (arrowModel == NULL) {
        arrowModel = new Model("arrow");
    }

    return arrowModel;
}

Arrow::Arrow() : GameObject(getArrowModel(), GameObject::Arrow) {
    modelTranslateOffset = glm::vec4(-scaleVector.x, 0, 0, 0);
    modelRotationOffset = glm::vec4(0,-PI/2,0,0);
    collisionType = Point;
    disapearTime = time(0) + 15;
}

void Arrow::update() {
    if (difftime(time(0),disapearTime) > 0.0) {
      mustDisapear = true;
    }

    float currentTime = glfwGetTime();
    float delta = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;

    if (floorColliding) {
        accelerationVector.y = 0;
        velocityVector.y = 0;
        phi = 0;
    }

    velocityVector += accelerationVector * delta;
    positionVector += velocityVector * delta * 4.0f;
    
    if (norm(velocityVector) != 0.0){
        setRotation(velocityVector);
    }
}