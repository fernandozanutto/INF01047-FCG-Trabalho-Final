#include <iostream>

#include "PrimaryActionCommand.h"


PrimaryActionCommand::PrimaryActionCommand(Game& game) : game(game) {
    repeatable = false;
}

void PrimaryActionCommand::onPress() {
    std::cout << "Press left mouse " << std::endl;
}

void PrimaryActionCommand::onRelease() {

}
