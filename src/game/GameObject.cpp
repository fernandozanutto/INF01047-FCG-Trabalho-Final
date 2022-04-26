#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

#include <iostream>

#include "GameObject.h"
#include "../renderer/Matrices.h"

#define PI 3.1415926f


GameObject::GameObject(Model* model): model(model) {
    resetMatrix();
}

void GameObject::resetMatrix() {
    modelOffset = glm::vec4(0);
    positionVector = glm::vec4(0.0, 0.0, 0.0, 1.0);
    rotationVector = glm::vec4(0.0);
    scaleVector = glm::vec4(1.0);
    velocityVector = glm::vec4(0.0);
    accelerationVector = glm::vec4(0.0);
    angularVelocityVector = glm::vec4(0.0);
    theta = 0.0f;
    phi = 0.0f;
    lastUpdateTime = glfwGetTime();
}

GameObject* GameObject::translate(float x, float y, float z) {
    positionVector += glm::vec4(x, y, z, 0);
    return this;
}

GameObject* GameObject::scale(float x, float y, float z) {
    scaleVector *= glm::vec4(x, y, z, 0);
    return this;
}

GameObject* GameObject::rotateX(float angle) {
    rotationVector += glm::vec4(angle, 0, 0, 0);
    return this;
}

GameObject* GameObject::rotateY(float angle) {
    rotationVector += glm::vec4(0, angle, 0, 0);
    return this;
}

GameObject* GameObject::rotateZ(float angle) {
    rotationVector += glm::vec4(0, 0, angle, 0);
    return this;
}

GameObject* GameObject::setVelocity(float x, float y, float z) {
    velocityVector = glm::vec4(x, y, z, 0);
    return this;
}

GameObject* GameObject::setAcceleration(float x, float y, float z) {
    accelerationVector = glm::vec4(x, y, z, 0);
    return this;
}

GameObject* GameObject::setAngularVelocity(float x, float y, float z) {
    angularVelocityVector = glm::vec4(x, y, z, 0);
    return this;
}

glm::mat4 GameObject::getModelMatrix() {
    return Matrix_Translate(positionVector.x, positionVector.y, positionVector.z)
            * Matrix_Rotate_Z(rotationVector.z)
            * Matrix_Rotate_X(rotationVector.x) 
            * Matrix_Rotate_Y(rotationVector.y)
            * Matrix_Scale(scaleVector.x, scaleVector.y, scaleVector.z)
            * Matrix_Translate(modelOffset.x, modelOffset.y, modelOffset.z);
}

glm::mat4 GameObject::getModelMatrixWithOffset() {
    return Matrix_Translate(positionVector.x, positionVector.y, positionVector.z)
            * Matrix_Rotate_Z(rotationVector.z)
            * Matrix_Rotate_X(rotationVector.x) 
            * Matrix_Rotate_Y(rotationVector.y)
            * Matrix_Scale(scaleVector.x, scaleVector.y, scaleVector.z);
}

Model* GameObject::getModel() {
    return model;
}

glm::vec4 GameObject::getFacingDirection() {
    glm::vec4 direction = glm::vec4(
            glm::cos(phi) * glm::sin(theta),
            glm::sin(phi),
            glm::cos(phi) * glm::cos(theta),
            0
    );

    return direction;
}

void GameObject::update() {
    float currentTime = glfwGetTime();
    float delta = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;

    float oldPhi = phi;

    if (floorColliding) {
        accelerationVector.y = 0;
        velocityVector.y = 0;
        phi = 0;
    }

    velocityVector += accelerationVector * delta;
    positionVector += velocityVector * delta;
    rotationVector += angularVelocityVector * delta;

    if (isWalkingLeft) {
        theta += PI / 2;
        float oldPhi = phi;
        phi = 0;
        positionVector += getFacingDirection() * walkSpeed * delta;
        theta -= PI / 2;
        phi = oldPhi;
    }

    if (isWalkingRight) {
        theta -= PI / 2;
        float oldPhi = phi;
        phi = 0;
        positionVector += getFacingDirection() * walkSpeed * delta;
        theta += PI / 2;
        phi = oldPhi;
    }

    if (isWalkingForward) {
        positionVector += getFacingDirection() * walkSpeed * delta;
    }

    if (isWalkingBackward) {
        positionVector -= getFacingDirection() * walkSpeed * delta;
    }

    phi = oldPhi;
}

void GameObject::changePlayerFacingDirection(float x, float y) {
    theta -= x;
    phi -= y;

    if(phi > PI/2) phi = PI/2;
    if(phi < -PI/2) phi = -PI/2;
}

glm::vec4 GameObject::getPosition() {
    return positionVector;
}

bool GameObject::hasCollision() {
    return collision;
}

bool GameObject::hasGravity() {
    return gravity;
}

std::vector<BoundingBox> GameObject::getGlobalBoundingBoxes() {
    std::vector<BoundingBox> globalBoudingBoxes;

    for(BoundingBox& box : model->boundingBoxes) {
        auto min = getModelMatrix() * (box.min + modelOffset);
        auto max = getModelMatrix() * (box.max + modelOffset);
        globalBoudingBoxes.push_back(BoundingBox(min, max));
    }

    return globalBoudingBoxes;
}

void GameObject::setBoundingBoxes(std::vector<BoundingBox> boundingBoxes) {
    model->boundingBoxes = boundingBoxes;
}