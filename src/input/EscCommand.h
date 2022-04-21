#pragma once

#include "Command.h"
#include "../game/Game.h"

class EscCommand: public Command {
private:
    Game& game;
public:
    EscCommand(Game& game);
    virtual void onPress() override;
    virtual void onRelease() override;
};