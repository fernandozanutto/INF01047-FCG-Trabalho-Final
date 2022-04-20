#pragma once

#include "BaseScene.h"

class Game {
private:
    bool isRunning;
    BaseScene currentScene;
public:
    Game(BaseScene& firstScene);
    void update();
    void setScene(BaseScene& scene);
    BaseScene& getScene();
};