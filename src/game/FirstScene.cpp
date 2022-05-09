#include <cstdlib>
#include <math.h>       /* atan2 */
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
#define LIMIT 40
#define NUMBER_TARGETS 10

FirstScene::FirstScene() {
    Model* bunnyModel = new Model("bunny", Model::PLANAR);

    Arrow arrow;

    GameObject* sphere = new GameObject(new Model("sphere", Model::SPHERIC));
    sphere->translate(-1.0f, 10, 0)->setAngularVelocity(0, .3f, 0);

    GameObject* bunny = new GameObject(bunnyModel);
    bunny->translate(1,10,0)->setAngularVelocity(0, .2f, .0f);
    
    GameObject* smallBunny = new GameObject(bunnyModel);
    smallBunny->scale(0.3f, 0.3f, 0.3f)->translate(0.0f, 10, 0.0f);

    for (int i = 0; i < NUMBER_TARGETS; i++) {
        Target* target = new Target;
        int x = rand() % ((LIMIT * 2) -1) + (-LIMIT);
        int y = rand() % ((LIMIT * 2) -1) + (-LIMIT);
        int z = rand() % ((LIMIT * 2) -1) + (-LIMIT);

        target->translate(x,y,z);
        this->gameObjects.push_back(target);
    }

    Model* dragonModel = new Model("dragon");

    glm::vec4 p1 = glm::vec4(0.f, 12.f,  -50.f, 1.f);
    glm::vec4 p2 = glm::vec4(2.f, -18.f, -50.f, 1.f);
    glm::vec4 p3 = glm::vec4(7.f, 22.f, -50.f, 1.f);
    glm::vec4 p4 = glm::vec4(10.f, 4.f,  -50.f, 1.f);
    BezierObject* dragon1 = new BezierObject(dragonModel, p1, p2, p3, p4, 4);
    dragon1->scale(.5f, .5f, .5f);

    p1 = glm::vec4(0.f, 32.f,  10.f, 1.f);
    p2 = glm::vec4(2.f, 48.f, 0.f, 1.f);
    p3 = glm::vec4(7.f, 42.f, 15.f, 1.f);
    p4 = glm::vec4(10.f, 44.f,  0.f, 1.f);
    BezierObject* dragon2 = new BezierObject(dragonModel, p1, p2, p3, p4, 4);
    
    this->gameObjects.push_back(sphere);
    this->gameObjects.push_back(dragon1);
    this->gameObjects.push_back(dragon2);
    this->gameObjects.push_back(bunny);
    this->gameObjects.push_back(smallBunny);

    GameObject* plane = new GameObject(new Model("plane", Model::REPEAT));
    plane->scale(50.0f, 2.0f, 50.0f);
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
