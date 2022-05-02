#pragma once

#include "Command.h"
#include "../game/Game.h"

class PrimaryActionCommand : public Command {
private:
    Game& game;
public:
    PrimaryActionCommand(Game& game);
    virtual void onPress() override;
    virtual void onRelease() override;
};