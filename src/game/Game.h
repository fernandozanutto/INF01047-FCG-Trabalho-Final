#pragma once

#include "BaseScene.h"

class Game {
private:
    bool isRunning;
public:
    Game(BaseScene* firstScene);
    BaseScene* currentScene;
    void update();

};