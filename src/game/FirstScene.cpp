#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

#include "FirstScene.h"
#include "GameObject.h"
#include "../renderer/Model.h"

FirstScene::FirstScene() {
    Model* bunnyModel = new Model("bunny");

    GameObject sphere(new Model("sphere"));
    sphere.rotateX(0.2f)->rotateZ(0.6f)->translate(-1.0f, 0, 0)->setAngularVelocity(0, .3f, 0);
    sphere.walkSpeed = 2;

    GameObject bunny(bunnyModel);
    bunny.translate(1,0,0)->setAngularVelocity(0, .2f, .0f);
    
    GameObject smallBunny(bunnyModel);
    smallBunny.scale(0.3f, 0.3f, 0.3f)->translate(0.0f, 2.0f, 0.0f);
    
    GameObject plane(new Model("plane"));
    plane.scale(2.0f, 1.0f, 2.0f)->translate(0, -1.1f, 0);
    
    this->gameObjects.push_back(sphere);
    this->gameObjects.push_back(bunny);
    this->gameObjects.push_back(plane);
    this->gameObjects.push_back(smallBunny);
}