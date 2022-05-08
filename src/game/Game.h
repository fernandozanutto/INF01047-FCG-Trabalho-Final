#pragma once

#include "BaseScene.h"
#include "entities/Player.h"

class Game {
private:
    bool isRunning;
    BaseScene& currentScene;
    bool checkCollision(GameObject*, GameObject*);
public:
    Game(BaseScene& firstScene, Player* player);
    Player* player;
    void update();
    void setScene(BaseScene& scene);
    BaseScene& getScene();
    GameObject* cameraFollowing;
    glm::vec4 getCameraPosition();
    void setPlayerMovingForward(bool);
    void setPlayerMovingLeft(bool);
    void setPlayerMovingBackward(bool);
    void setPlayerMovingRight(bool);
    void changePlayerFacingDirection(float x, float y);
    void executeMainAction();
    void pause();
    bool hasFinished = false;
    void reset();
};