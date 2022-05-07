#pragma once

#include <glm/mat4x4.hpp>

#include "../renderer/Model.h"

class GameObject {
protected:
    Model* model;
    // all velocities and accelerations are in units/second
    glm::vec4 positionVector;
    glm::vec4 rotationVector;
    glm::vec4 scaleVector; 
    glm::vec4 velocityVector;
    glm::vec4 accelerationVector;
    glm::vec4 angularVelocityVector; // em radianos
    float lastUpdateTime;
    bool collision = true;
    glm::vec4 modelTranslateOffset;
    glm::vec4 modelRotationOffset;
    
public:
    int id;
    enum ObjectType { Arrow, Comum };
    enum CollisionType { Point, Plane, OBB, Sphere };
    GameObject(Model* model, ObjectType type = ObjectType::Comum);
    GameObject* translate(float x, float y, float z);
    GameObject* scale(float x, float y, float z);
    GameObject* rotateX(float angle);
    GameObject* rotateY(float angle);
    GameObject* rotateZ(float angle);
    GameObject* setVelocity(float x, float y, float z);
    GameObject* setAcceleration(float x, float y, float z);
    GameObject* setAngularVelocity(float x, float y, float z);
    GameObject* setGlobalPosition(float x, float y, float z);

    void changeFacingDirection(float dx, float dy);
    void setRotation(float phi, float theta);
    void setBoundingBoxes(std::vector<BoundingBox>);

    ObjectType objectType;
    CollisionType collisionType = OBB;

    bool isWalkingForward = false;
    bool isWalkingLeft = false;
    bool isWalkingBackward = false;
    bool isWalkingRight = false;
    bool gravity = false;

    float theta;     // Ângulo no plano ZX em relação ao eixo Z
    float phi;       // Ângulo em relação ao eixo Y

    bool isPlayerWalking();
    bool hasCollision();
    bool hasGravity();

    bool floorColliding = false;

    Model* getModel();
    void resetMatrix();
    virtual void update();
    glm::mat4 getModelMatrix();
    glm::mat4 getModelMatrixWithOffset();
    glm::vec4 getFacingDirection();
    glm::vec4 getPosition();
    
    float walkSpeed;

    bool rotateModelOnCameraChange = false;

    std::vector<BoundingBox> getGlobalBoundingBoxes();
};