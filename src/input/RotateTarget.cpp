#include "RotateTarget.h"
#include "../game/Game.h"
#include <iostream>

RotateTarget::RotateTarget(Game& game, Side side) : game(game), side(side) { }

void rotate(bool side, Game& game) {
  for (GameObject* obj : game.getScene().gameObjects) {
    if (obj->objectType == GameObject::ObjectType::Target) {
      if (side && !obj->isPlayer)
      {
        obj->setAngularVelocity(0, -.5f, .0f);
      } else {
        obj->setAngularVelocity(0, .5f, .0f);
      }      
    }
  }
}

void RotateTarget::onPress() {
    switch(side) {
      case(LEFT):
          rotate(true, game);
          break;
      case(RIGHT):
          rotate(false, game);
          break;
  }
}

void RotateTarget::onRelease() {
}