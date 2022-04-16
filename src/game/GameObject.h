#pragma once

#include <glm/mat4x4.hpp>

#include "../renderer/Model.h"

class GameObject {
public:
    GameObject();
    GameObject(Model* model);
    Model* model;
    glm::mat4 modelMatrix;

    GameObject* translate(float x, float y, float z);
    GameObject* scale(float x, float y, float z);
    GameObject* rotateX(float angle);
    GameObject* rotateY(float angle);
    GameObject* rotateZ(float angle);
};