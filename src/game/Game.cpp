#include <iostream>

#include <glm/vec4.hpp>

#include "Game.h"
#include "BaseScene.h"

Game::Game(BaseScene& firstScene, GameObject& player) : currentScene(firstScene), player(player) {
    isRunning = true;
    cameraFollowing = &player;
}

void Game::update() {
    if (!isRunning) return;

    for (GameObject& object : currentScene.gameObjects) {
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