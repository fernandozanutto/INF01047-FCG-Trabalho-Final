#include "Game.h"
#include "BaseScene.h"

#include <iostream>

Game::Game(BaseScene& firstScene): currentScene(firstScene) {
    isRunning = true;
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