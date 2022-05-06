#pragma once

#include "Command.h"
#include "../renderer/Renderer.h"

class ReloadShadersCommand : public Command {
private:
    Renderer& renderer;
    Game& game;
public:
    ReloadShadersCommand(Renderer& renderer, Game& game);
    virtual void onPress() override;
    virtual void onRelease() override;
};