#include "EscCommand.h"
#include "../game/Game.h"


EscCommand::EscCommand(Game& game) : game(game) {
    repeatable = false;
}

void EscCommand::onPress() {
    game.pause();
}

void EscCommand::onRelease() {}