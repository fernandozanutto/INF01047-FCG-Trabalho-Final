#include "MoveCommand.h"
//#include "Game.h"

MoveCommand::MoveCommand(Direction d) : dir(d) { }
void MoveCommand::execute() {
    switch(dir) {
        case(FORWARD):
            //game.movePlayerForward();
            break;
        case(BACKWARD):
            //game.movePlayerBackward();
            break;
        case(LEFT):
            //game.movePlayerLeft();
            break;
        case(RIGHT):
            //game.movePlayerRight();
            break;
    }
}