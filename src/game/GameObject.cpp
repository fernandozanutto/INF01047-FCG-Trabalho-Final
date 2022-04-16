#include "GameObject.h"
#include "../renderer/Matrices.h"

GameObject::GameObject() {
    this->modelMatrix = Matrix_Identity();
    this->model = NULL;
}

GameObject::GameObject(Model* model): model(model) {
    this->modelMatrix = Matrix_Identity();
}

GameObject* GameObject::translate(float x, float y, float z) {
    this->modelMatrix = Matrix_Translate(x, y, z) * this->modelMatrix;
    return this;
}

GameObject* GameObject::scale(float x, float y, float z) {
    this->modelMatrix = Matrix_Scale(x, y, z) * this->modelMatrix;
    return this;
}

GameObject* GameObject::rotateX(float angle) {
    this->modelMatrix = Matrix_Rotate_X(angle) * this->modelMatrix;
    return this;
}

GameObject* GameObject::rotateY(float angle) {
    this->modelMatrix = Matrix_Rotate_Y(angle) * this->modelMatrix;
    return this;
}

GameObject* GameObject::rotateZ(float angle) {
    this->modelMatrix = Matrix_Rotate_Z(angle) * this->modelMatrix;
    return this;
}