#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <iostream>
#include "GameObject.h"
#include "../renderer/Matrices.h"

float lastFrameTime = glfwGetTime();
float startTime = glfwGetTime();

GameObject::GameObject() {
    model = NULL;
    std::cout << "construtor padrao" << std::endl;
    resetMatrix();
}

GameObject::GameObject(Model* model) {
    this->model = model;
    resetMatrix();
    std::cout << "construtor com model " << this << " " << this->model << std::endl;
}

void GameObject::resetMatrix() {
    positionVector = glm::vec4(0.0, 0.0, 0.0, 0.0);
    rotationVector = glm::vec4(0.0);
    scaleVector = glm::vec4(1.0);
    velocityVector = glm::vec4(0.0, 0.0, 0.0, 0.0);
    accelerationVector = glm::vec4(0.0, 0.0, 0.0, 0.0);
    angularVelocityVector = glm::vec4(0.0, 0.0, 0.0, 0.0);
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
            * Matrix_Rotate_X(rotationVector.x) 
            * Matrix_Rotate_Y(rotationVector.y)
            * Matrix_Rotate_Z(rotationVector.z)
            * Matrix_Scale(scaleVector.x, scaleVector.y, scaleVector.z);
}

Model* GameObject::getModel() {
    return model;
}

void GameObject::update() {
    if (model == NULL) {
        std::cout << "NULOOOOO" << std::endl;
        return;
    }
    std::string name = model->name;
    
    float currentTime = glfwGetTime();
    float delta = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    glm::vec4 velocity = velocityVector * delta;

    if (name == "sphere") {
        std::cout << " old X position " << this->positionVector.x << " - velocity X: " << velocity.x;
    }

    this->velocityVector += accelerationVector * delta;
    this->positionVector += velocity;
    this->rotationVector += angularVelocityVector * delta;


    if (name == "sphere") {
        std::cout << " - new X position: " << this->positionVector.x << " " << this << std::endl;
    }
}