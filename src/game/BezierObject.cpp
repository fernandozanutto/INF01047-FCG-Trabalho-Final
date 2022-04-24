

#include <cmath>
#include <iostream>

#include <GLFW/glfw3.h>

#include "BezierObject.h"

BezierObject::BezierObject(Model* model, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4, float period)
: p1(p1), p2(p2), p3(p3), p4(p4), movePeriod(period), GameObject(model) {
    lastUpdate = glfwGetTime();
    startTime = lastUpdate;
    gravity = false;
}


void BezierObject::update() {
    float currentTime = glfwGetTime();
    float delta = currentTime - lastUpdate;
    lastUpdate = currentTime;

    float sinceStart = currentTime - startTime;
    int currentCycle = sinceStart / movePeriod;

    float cycleTime = sinceStart;
    while (cycleTime >= movePeriod) {
        cycleTime -= movePeriod;
    }

    float currentT = cycleTime / movePeriod;
    if (currentCycle % 2 == 1) {
        currentT = (1 - currentT);
    }

    float b03 = pow(1.0f - currentT, 3);
    float b13 = 3 * currentT * pow(1.0f - currentT, 2);
    float b23 = 3 * pow(currentT, 2) * (1.0f - currentT);
    float b33 = pow(currentT, 3) ;

    glm::vec4 c = b03 * p1 + b13 * p2 + b23 * p3 + b33 * p4;

    //std::cout << "X: " << c.x << " Y: " << c.y << " Z: " << c.z << " t: " << currentT << " currentCycle: " << currentCycle << std::endl;
    this->positionVector = c;
}