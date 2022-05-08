#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

#include "FirstScene.h"
#include "GameObject.h"
#include "BezierObject.h"
#include "entities/Arrow.h"
#include "entities/Target.h"
#include "entities/Player.h"
#include "../renderer/Model.h"

#define PI 3.1415926f

FirstScene::FirstScene() {
    Model* bunnyModel = new Model("bunny", Model::PLANAR);

    GameObject* sphere = new GameObject(new Model("sphere", Model::SPHERIC));
    sphere->translate(-1.0f, 10, 0)->setAngularVelocity(0, .3f, 0);

    GameObject* bunny = new GameObject(bunnyModel);
    bunny->translate(1,10,0)->setAngularVelocity(0, .2f, .0f);
    
    GameObject* smallBunny = new GameObject(bunnyModel);
    smallBunny->scale(0.3f, 0.3f, 0.3f)->translate(0.0f, 10, 0.0f);

    Target* target = new Target;
    target->translate(1,10,0);

    Target* target2 = new Target;
    target2->translate(1,1,5);

    Target* target3 = new Target;
    target3->translate(4,1,4);

    Target* target4 = new Target;
    target4->translate(10,1,3);

    Model* targetModel = new Model("target", Model::PLANAR);

    glm::vec4 p1 = glm::vec4(0.f, 12.f,  5.f, 1.f);
    glm::vec4 p2 = glm::vec4(2.f, -18.f, 5.f, 1.f);
    glm::vec4 p3 = glm::vec4(7.f, 22.f, 5.f, 1.f);
    glm::vec4 p4 = glm::vec4(10.f, 4.f,  5.f, 1.f);
    BezierObject* robot2 = new BezierObject(targetModel, p1, p2, p3, p4, 4);
    robot2->scale(.5f, .5f, .5f);
    
    this->gameObjects.push_back(sphere);
    this->gameObjects.push_back(robot2);
    this->gameObjects.push_back(bunny);
    this->gameObjects.push_back(smallBunny);
    this->gameObjects.push_back(target);
    this->gameObjects.push_back(target2);
    this->gameObjects.push_back(target3);
    this->gameObjects.push_back(target4);

    GameObject* plane = new GameObject(new Model("plane", Model::REPEAT));
    plane->scale(10.0f, 2.0f, 20.0f);
    plane->collisionType = GameObject::Plane;
    plane->objectType == GameObject::Floor;
    this->floor = plane;


    Model* robotModel = new Model("robot", Model::PLANAR);
    GameObject* robot = new GameObject(robotModel, GameObject::Player);
    robot->rotateModelOnCameraChange = true;
    robot->walkSpeed = 5;
    robot->scale(.7f, .7f, 0.7f);

    this->player = new Player(robot);
    
}
