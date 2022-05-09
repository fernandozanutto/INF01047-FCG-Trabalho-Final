#include <iostream>

#include "SecondaryActionCommand.h"


SecondaryActionCommand::SecondaryActionCommand(Game& game) : game(game) {
    repeatable = true;
}

void SecondaryActionCommand::onPress() {
    game.lookAtCamera = true;
}

void SecondaryActionCommand::onRelease() {
    game.lookAtCamera = false;
}
