#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

#include "FirstScene.h"
#include "GameObject.h"
#include "BezierObject.h"
#include "entities/Arrow.h"
#include "entities/Target.h"
#include "../renderer/Model.h"

FirstScene::FirstScene() {
    Model* bunnyModel = new Model("bunny", Model::PLANAR);

    GameObject* sphere = new GameObject(new Model("sphere", Model::SPHERIC));
    sphere->translate(-1.0f, 10, 0)->setAngularVelocity(0, .3f, 0);
    sphere->walkSpeed = 2;

    GameObject* bunny = new GameObject(bunnyModel);
    bunny->translate(1,10,0)->setAngularVelocity(0, .2f, .0f);
    
    GameObject* smallBunny = new GameObject(bunnyModel);
    smallBunny->scale(0.3f, 0.3f, 0.3f)->translate(0.0f, 10, 0.0f);

    Target* target = new Target;
    target->translate(1,10,0)->setAngularVelocity(0, .2f, .0f)->scale(0.3f, 0.3f, 0.3f);

    Model* robotModel = new Model("robot");

    GameObject* robot = new GameObject(robotModel);
    robot->translate(3, 10, 3);

    glm::vec4 p1 = glm::vec4(0.f, 12.f,  5.f, 1.f);
    glm::vec4 p2 = glm::vec4(2.f, -18.f, 5.f, 1.f);
    glm::vec4 p3 = glm::vec4(7.f, 22.f, 5.f, 1.f);
    glm::vec4 p4 = glm::vec4(10.f, 4.f,  5.f, 1.f);
    BezierObject* robot2 = new BezierObject(robotModel, p1, p2, p3, p4, 3);
    robot2->scale(.5f, .5f, .5f);

    Arrow* arrow = new Arrow;
    arrow->setAngularVelocity(0,3.14,0)->translate(-2,3,-2);

    Arrow* arrow2 = new Arrow;
    arrow2->walkSpeed = 3;
    arrow2->rotateModelOnCameraChange = true;

    this->gameObjects.push_back(arrow2);
    this->gameObjects.push_back(sphere);
    this->gameObjects.push_back(arrow);
    this->gameObjects.push_back(robot2);
    this->gameObjects.push_back(bunny);
    this->gameObjects.push_back(smallBunny);
    this->gameObjects.push_back(robot);
    this->gameObjects.push_back(target);

    GameObject* plane = new GameObject(new Model("plane", Model::REPEAT));
    
    plane->scale(10.0f, 1.0f, 20.0f)->translate(0, 0, 0);
    plane->collisionType = GameObject::Plane;
    
    this->floor = plane;
}