#include <map>
#include <iterator>

//#include "Player.h"
#include "InputManager.h" 
#include <GLFW/glfw3.h>

#include <iostream>

InputManager::InputManager(std::vector<std::tuple<int, Command*>> cList, Game& game): game(game) {
    for(unsigned int i=0; i < cList.size(); i++) {
        int key          = std::get<0>(cList[i]);
        Command* command = std::get<1>(cList[i]);
        heldKeys[key] = false;
        commands[key] = command;
    }
}

void InputManager::keyCallback(int key, int action, int mods) {
    bool keyState;
    if(action == GLFW_PRESS || action == GLFW_REPEAT){
        keyState = true;
    }
    else if(action == GLFW_RELEASE){
        keyState = false;
    }

    std::map<int, bool>::iterator it = heldKeys.find(key); 
    if (it != heldKeys.end())
        it->second = keyState; 
}

void InputManager::mouseButtonCallback(int button, int action, int mods) {
    bool keyState;

    if(action == GLFW_PRESS) {
        keyState = true;
    } else if(action == GLFW_RELEASE) {
        keyState = false;
    }
    
    std::map<int, bool>::iterator it = heldKeys.find(button); 
    if (it != heldKeys.end()) {
        it->second = keyState;
    }
}
    
void InputManager::cursorCallback(double xpos, double ypos) {
    float dx = xpos - lastCursorPosX;
    float dy = ypos - lastCursorPosY;
    lastCursorPosX = xpos;
    lastCursorPosY = ypos;

    float sensibility = 0.01;

    game.changePlayerFacingDirection(dx * sensibility, dy * sensibility);
}

void InputManager::setInitialCursorPos(double xpos, double ypos) {
    lastCursorPosX = xpos;
    lastCursorPosY = ypos;
}

void InputManager::handleInput() {
    for(auto& k : heldKeys) {
        Command* targetCommand = commands[k.first];
        if(k.second){
            targetCommand->onPress();
            if(!targetCommand->isRepeatable()){
                k.second = false;
            }
        } else {
            targetCommand->onRelease();
        }
    }
}