#include <iostream>

#include "PrimaryActionCommand.h"


PrimaryActionCommand::PrimaryActionCommand(Game& game) : game(game) {
    repeatable = false;
}

void PrimaryActionCommand::onPress() {
    game.executeMainAction();
}

void PrimaryActionCommand::onRelease() {

}
