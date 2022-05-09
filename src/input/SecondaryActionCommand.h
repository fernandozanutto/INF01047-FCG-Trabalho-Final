#pragma once

#include "Command.h"
#include "../game/Game.h"

class SecondaryActionCommand : public Command {
private:
    Game& game;
public:
    SecondaryActionCommand(Game& game);
    virtual void onPress() override;
    virtual void onRelease() override;
};