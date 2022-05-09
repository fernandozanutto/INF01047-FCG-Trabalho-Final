#pragma once
#include "../game/Game.h"
#include "Command.h"

class RotateTarget: public Command {
public:
    enum Side {LEFT, RIGHT};
    RotateTarget(Game& game, Side side);
    Game& game;
    Side side;
    virtual void onPress() override;
    virtual void onRelease() override;
protected:
};