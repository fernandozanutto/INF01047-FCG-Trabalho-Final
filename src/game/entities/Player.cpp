#include <iostream>
#include <time.h>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

#include "Player.h"
#include "../../renderer/Model.h"
#include "../../renderer/Matrices.h"

#define PI 3.1415926f

Player::Player(GameObject* gameObject) : gameObject(gameObject) {
}