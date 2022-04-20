#pragma once

#include <glm/mat4x4.hpp>

#include "../renderer/Model.h"

class GameObject {
private:
    Model* model;
    // all velocities and accelerations are in units/second
    glm::vec4 positionVector;
    glm::vec4 rotationVector;
    glm::vec4 scaleVector; 
    glm::vec4 velocityVector;
    glm::vec4 accelerationVector;
    glm::vec4 angularVelocityVector;
    

public:
    GameObject();
    GameObject(Model* model);
    
    Model* getModel();
    
    void resetMatrix();
    GameObject* translate(float x, float y, float z);
    GameObject* scale(float x, float y, float z);
    GameObject* rotateX(float angle);
    GameObject* rotateY(float angle);
    GameObject* rotateZ(float angle);
    GameObject* setVelocity(float x, float y, float z);
    GameObject* setAcceleration(float x, float y, float z);
    GameObject* setAngularVelocity(float x, float y, float z);
    glm::mat4 getModelMatrix();

    void update();
};