#pragma once

#include "BaseScene.h"

class Game {
private:
    bool isRunning;
    BaseScene& currentScene;
public:
    Game(BaseScene& firstScene, GameObject& player);
    GameObject& player;
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
};