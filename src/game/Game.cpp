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

Game::Game(BaseScene& firstScene, GameObject& player) : currentScene(firstScene), player(player) {
    isRunning = true;
    cameraFollowing = &player;
}

bool Game::checkCollision(GameObject* object1, GameObject* object2) {
    if (!object1->hasCollision() || !object2->hasCollision()) return false;
    //std::cout << (object1->collisionType) << (object2->collisionType) << "---" << (object1->collisionType == GameObject::Point && object2->collisionType == GameObject::OBB) << "\n";

    if (object1->collisionType == GameObject::Point && object2->collisionType == GameObject::OBB) {
        return pointBoundingBoxCollision(object1->getPosition(), object2->getGlobalBoundingBoxes()[0]);
    } else if (object1->collisionType == GameObject::Point && object2->collisionType == GameObject::Plane) {
        return pointPlaneCollision(object1->getPosition(), object2->getGlobalBoundingBoxes()[0]);
    } else if (object1->collisionType == GameObject::OBB && object2->collisionType == GameObject::Plane) {
        return boundBoxPlaneCollision(object1->getGlobalBoundingBoxes()[0], object2->getGlobalBoundingBoxes()[0]);
    } else if (object1->collisionType == GameObject::OBB && object2->collisionType == GameObject::OBB){
        return boundBoxCollision(object1->getGlobalBoundingBoxes()[0], object2->getGlobalBoundingBoxes()[0]);
    }

    return false;
}

void Game::reset() {
    currentScene = FirstScene();
    player = *currentScene.gameObjects[0];
}

void Game::update() {
    if (!isRunning) return;

    currentScene.floor->update();

    for (GameObject* object : currentScene.gameObjects) {
        if (object->hasGravity()) {
            object->setAcceleration(0, -9.8f, 0);
        }

        if (checkCollision(object, currentScene.floor)) {
            object->floorColliding = true;
        } else {
            object->floorColliding = false;
        }

        for (GameObject* target : currentScene.gameObjects) {
          if(target->objectType == GameObject::ObjectType::Comum && object->objectType == GameObject::ObjectType::Arrow ||
          object->objectType == GameObject::ObjectType::Comum && target->objectType == GameObject::ObjectType::Arrow) {
            if (checkCollision(object, target) || checkCollision(target, object)) {
              currentScene.removeObject(target);
            }
          }
        }

        object->update();
        if (object->mustDisapear) 
        {
          currentScene.removeObject(object);
        }
        
    }
}

void Game::setScene(BaseScene& scene) {
    currentScene = scene;
}

BaseScene& Game::getScene() {
    return currentScene;
}

void Game::setPlayerMovingForward(bool isMoving) {
    player.isWalkingForward = isMoving;
}

void Game::setPlayerMovingLeft(bool isMoving) {
    player.isWalkingLeft = isMoving;
}

void Game::setPlayerMovingBackward(bool isMoving) {
    player.isWalkingBackward = isMoving;
}

void Game::setPlayerMovingRight(bool isMoving) {
    player.isWalkingRight = isMoving;
}

void Game::changePlayerFacingDirection(float x, float y) {
    player.changeFacingDirection(x, y);
}

glm::vec4 Game::getCameraPosition() {
    bool firstPerson = true;
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
    std::cout << "Press left mouse " << std::endl;

    Arrow* newArrow = new Arrow;
    glm::vec4 facingDirection = player.getFacingDirection();
    glm::vec4 position = player.getPosition();

    float maxY = player.getGlobalBoundingBoxes()[0].max.y;
    position.y = maxY;

    newArrow->setGlobalPosition(position.x, position.y, position.z)->setVelocity(facingDirection.x * 15,facingDirection.y * 15,facingDirection.z * 15)->scale(0.5,0.5,0.5);
    newArrow->setRotation(facingDirection);
    
    currentScene.gameObjects.push_back(newArrow);
}