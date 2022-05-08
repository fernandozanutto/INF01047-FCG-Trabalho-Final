#include "BaseScene.h"
#include <iostream>


void BaseScene::removeObject(GameObject* object) {
  int counter = 0;

  //std::cout << object->getModel()->name << "3";
  for (GameObject* obj : gameObjects) {
    if (obj->id == object->id) {
      gameObjects.erase(gameObjects.begin() + counter);
    }

    counter++;
  }
}
