#pragma once

#include <vector>

#include "GameObject.h"
#include "entities/Player.h"

class BaseScene {
public:
    std::vector<GameObject*> gameObjects;
    GameObject* floor;
    Player* player;
    void removeObject(GameObject* object);
    void spawnNewTarget();
};
