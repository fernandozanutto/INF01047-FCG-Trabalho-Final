#pragma once

#include <vector>

#include "GameObject.h"

class BaseScene {
public:
    std::vector<GameObject*> gameObjects;
    GameObject* floor;
    void removeObject(GameObject* object);
};
