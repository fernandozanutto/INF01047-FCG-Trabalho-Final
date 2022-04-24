#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

#include "FirstScene.h"
#include "GameObject.h"
#include "BezierObject.h"
#include "../renderer/Model.h"

FirstScene::FirstScene() {
    Model* bunnyModel = new Model("bunny", Model::PLANAR);

    GameObject* sphere = new GameObject(new Model("sphere", Model::SPHERIC));
    sphere->translate(-1.0f, 1.f, 0)->setAngularVelocity(0, .3f, 0);
    sphere->walkSpeed = 2;

    GameObject* bunny = new GameObject(bunnyModel);
    bunny->translate(1,1.f,0)->setAngularVelocity(0, .2f, .0f);
    
    GameObject* smallBunny = new GameObject(bunnyModel);
    smallBunny->scale(0.3f, 0.3f, 0.3f)->translate(0.0f, 3.0f, 0.0f);
    

    Model* robotModel = new Model("robot");

    GameObject* robot = new GameObject(robotModel);
    robot->translate(3, 1, 3);

    glm::vec4 p1 = glm::vec4(0.f, 12.f,  5.f, 1.f);
    glm::vec4 p2 = glm::vec4(2.f, -18.f, 5.f, 1.f);
    glm::vec4 p3 = glm::vec4(7.f, 22.f, 5.f, 1.f);
    glm::vec4 p4 = glm::vec4(10.f, 4.f,  5.f, 1.f);
    BezierObject* robot2 = new BezierObject(robotModel, p1, p2, p3, p4, 3);
    robot2->scale(.5f, .5f, .5f);

    this->gameObjects.push_back(sphere);
    this->gameObjects.push_back(robot2);
    this->gameObjects.push_back(bunny);
    this->gameObjects.push_back(smallBunny);
    this->gameObjects.push_back(robot);


    GameObject* plane = new GameObject(new Model("plane"));
    plane->scale(10.0f, 1.0f, 10.0f)->translate(0, -1.f, 0);

    this->floor = plane;
}