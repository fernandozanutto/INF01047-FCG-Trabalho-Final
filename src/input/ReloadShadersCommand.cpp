
#include "ReloadShadersCommand.h"
#include "../renderer/Renderer.h"


ReloadShadersCommand::ReloadShadersCommand(Renderer& renderer) : renderer(renderer) {
    repeatable = false;
}

void ReloadShadersCommand::onPress() {
    renderer.loadShaders();
}

void ReloadShadersCommand::onRelease() {

}