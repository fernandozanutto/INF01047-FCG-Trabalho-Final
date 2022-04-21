#include <iostream>

#include "MoveCommand.h"
#include "../game/Game.h"

MoveCommand::MoveCommand(Game& game, Direction d) : dir(d), game(game) { }

void MoveCommand::onPress() {
    switch(dir) {
        case(FORWARD):
            game.setPlayerMovingForward(true);
            break;
        case(BACKWARD):
            game.setPlayerMovingBackward(true);
            break;
        case(LEFT):
            game.setPlayerMovingLeft(true);
            break;
        case(RIGHT):
            game.setPlayerMovingRight(true);
            break;
    }
}

void MoveCommand::onRelease() {
    switch(dir) {
        case(FORWARD):
            game.setPlayerMovingForward(false);
            break;
        case(BACKWARD):
            game.setPlayerMovingBackward(false);
            break;
        case(LEFT):
            game.setPlayerMovingLeft(false);
            break;
        case(RIGHT):
            game.setPlayerMovingRight(false);
            break;
    }
}