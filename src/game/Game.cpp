#include <iostream>

#include <glm/vec4.hpp>

#include "Game.h"
#include "BaseScene.h"
#include "collisions.h"

Game::Game(BaseScene& firstScene, GameObject& player) : currentScene(firstScene), player(player) {
    isRunning = true;
    cameraFollowing = &player;
}

void Game::update() {
    if (!isRunning) return;

    for (GameObject& object : currentScene.gameObjects) {

        if (object.hasGravity()) {
            object.setAcceleration(0, -9.8f, 0);
        }

        if (object.hasCollision() && boundBoxPlaneCollision(object.getGlobalBoundingBoxes()[0], currentScene.floor->getGlobalBoundingBoxes()[0])) {
            // std::cout << "colidindo com o chao" << std::endl;
            object.floorColliding = true;
        }

        object.update();
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
    player.changePlayerFacingDirection(x, y);
}

glm::vec4 Game::getCameraPosition() {
    return cameraFollowing->getPosition() - (cameraFollowing->getFacingDirection() * 3.0f);
}

void Game::pause() {
    isRunning = false;
    hasFinished = true;
}