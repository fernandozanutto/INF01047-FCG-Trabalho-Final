
#include "ReloadShadersCommand.h"
#include "../renderer/Renderer.h"


ReloadShadersCommand::ReloadShadersCommand(Renderer& renderer, Game& game) : renderer(renderer), game(game) {
    repeatable = false;
}

void ReloadShadersCommand::onPress() {
    renderer.loadShaders();
    //game.reset();
}

void ReloadShadersCommand::onRelease() {

}