#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/vec4.hpp>

#include "Game.h"
#include "BaseScene.h"
#include "GameObject.h"
#include "FirstScene.h"
#include "entities/Arrow.h"
#include "collisions.h"

Game::Game(BaseScene& firstScene, Player* player) : currentScene(firstScene), player(player) {
    isRunning = true;
    cameraFollowing = player->gameObject;
    player->gameObject->isPlayer = true;
}

bool applyTests(GameObject* object1, GameObject* object2) {
  if (object1->collisionType == GameObject::Point && object2->collisionType == GameObject::OBB) {
      return pointBoundingBoxCollision(object1->getPosition(), object2->getGlobalBoundingBoxes()[0]);
  } else if (object1->collisionType == GameObject::Point && object2->collisionType == GameObject::Plane) {
      return pointPlaneCollision(object1->getPosition(), object2->getGlobalBoundingBoxes()[0]);
  } else if (object1->collisionType == GameObject::OBB && object2->collisionType == GameObject::Plane) {
      return boundBoxPlaneCollision(object1->getGlobalBoundingBoxes()[0], object2->getGlobalBoundingBoxes()[0]);
  } else if (object1->collisionType == GameObject::OBB && object2->collisionType == GameObject::OBB) {
      return boundBoxCollision(object1->getGlobalBoundingBoxes()[0], object2->getGlobalBoundingBoxes()[0]);
  }

  return false;
}

bool Game::checkCollision(GameObject* object1, GameObject* object2) {
    if (!object1->hasCollision() || !object2->hasCollision()) return false;
    return (applyTests(object1, object2) || applyTests(object2, object1));
}

void Game::reset() {
    currentScene = FirstScene();
    player = currentScene.player;
}

void Game::update() {
    if (!isRunning) return;

    currentScene.floor->update();

    std::vector<GameObject*> updateObjects = currentScene.gameObjects;
    updateObjects.push_back(currentScene.player->gameObject);

    bool destroyedTarget = false;
    for (GameObject* object : updateObjects) {
        if (object->hasGravity() && object->getPosition().y > 0) {
            object->setAcceleration(0, -9.8f, 0);
        } else {
            object->setAcceleration(0, 0, 0);
            object->setVelocity(0, 0, 0);
        }

        if (checkCollision(object, currentScene.floor)) {
            object->floorColliding = true;
        } else {
            object->floorColliding = false;
        }

        for (GameObject* target : currentScene.gameObjects) {
          if(target->objectType == GameObject::ObjectType::Comum && object->objectType == GameObject::ObjectType::Arrow ||
          object->objectType == GameObject::ObjectType::Comum && target->objectType == GameObject::ObjectType::Arrow) {
            if (checkCollision(object, target) && !target->isPlayer) {
              target->mustDisapear = true;
              destroyedTarget = true;
            }
          }
        }

        object->update();
        if (object->mustDisapear) {
          currentScene.removeObject(object);
        }
    }

    if (destroyedTarget) {
        currentScene.spawnNewTarget();
    }
}

void Game::setScene(BaseScene& scene) {
    currentScene = scene;
}

BaseScene& Game::getScene() {
    return currentScene;
}

void Game::setPlayerMovingForward(bool isMoving) {
    player->gameObject->isWalkingForward = isMoving;
}

void Game::setPlayerMovingLeft(bool isMoving) {
    player->gameObject->isWalkingLeft = isMoving;
}

void Game::setPlayerMovingBackward(bool isMoving) {
    player->gameObject->isWalkingBackward = isMoving;
}

void Game::setPlayerMovingRight(bool isMoving) {
    player->gameObject->isWalkingRight = isMoving;
}

void Game::changePlayerFacingDirection(float x, float y) {
    player->gameObject->changeFacingDirection(x, y);
}

glm::vec4 Game::getCameraPosition() {
    bool firstPerson = false;
    auto maxY = cameraFollowing->getGlobalBoundingBoxes()[0].max.y;

    auto headPosition = cameraFollowing->getPosition();
    headPosition.y = maxY;
    glm::vec4 cameraPos;
    
    if (firstPerson) {
        cameraPos = headPosition;
    } else {
        cameraPos = headPosition - (cameraFollowing->getFacingDirection() * 5.0f);
    }
    
    return cameraPos;
}

void Game::pause() {
    isRunning = false;
    hasFinished = true;
}

void Game::executeMainAction() {
    Arrow* newArrow = new Arrow;
    glm::vec4 facingDirection = player->gameObject->getFacingDirection();
    glm::vec4 position = player->gameObject->getPosition();

    float maxY = player->gameObject->getGlobalBoundingBoxes()[0].max.y;
    position.y = maxY;

    newArrow->setGlobalPosition(position.x, position.y, position.z)->setVelocity(facingDirection.x * 15,facingDirection.y * 15,facingDirection.z * 15)->scale(0.5,0.5,0.5);
    newArrow->setRotation(facingDirection);
    
    currentScene.gameObjects.push_back(newArrow);
}
