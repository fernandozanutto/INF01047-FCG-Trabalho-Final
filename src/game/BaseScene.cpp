#include <iostream>

#include "BaseScene.h"
#include "entities/Target.h"

#define LIMIT 40

void BaseScene::removeObject(GameObject* object) {
    int counter = 0;

    for (GameObject* obj : gameObjects) {
        if (obj->id == object->id) {
            gameObjects.erase(gameObjects.begin() + counter);
        }
        
        counter++;
    }
}

void BaseScene::spawnNewTarget() {
    Target* target = new Target;
    int x = rand() % ((LIMIT * 2) -1) + (-LIMIT);
    int y = rand() % ((LIMIT * 2) -1) + (-LIMIT);
    int z = rand() % ((LIMIT * 2) -1) + (-LIMIT);

    target->translate(x,y,z);
    this->gameObjects.push_back(target);
}
