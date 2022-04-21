#pragma once

#include "Command.h"
#include "../renderer/Renderer.h"

class ReloadShadersCommand : public Command {
private:
    Renderer& renderer;
public:
    ReloadShadersCommand(Renderer& renderer);
    virtual void onPress() override;
    virtual void onRelease() override;
};