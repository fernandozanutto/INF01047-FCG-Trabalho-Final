#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

#include "Scene.h"
#include "GameObject.h"
#include "../renderer/Model.h"

Scene::Scene() {
    Model* bunnyModel = new Model("bunny");

    GameObject sphere(new Model("sphere"));
    sphere.rotateX(0.2f)->rotateZ(0.6f)->translate(-1.0f, 0, 0);

    GameObject bunny(bunnyModel);
    bunny.translate(1,0,0);

    GameObject smallBunny(bunnyModel);
    smallBunny.scale(0.3f, 0.3f, 0.3f)->translate(0.0f, 2.0f, 0.0f);

    GameObject plane(new Model("plane"));
    plane.scale(2.0f, 1.0f, 2.0f)->translate(0, -1.1f, 0);

    this->gameObject.push_back(sphere);
    this->gameObject.push_back(bunny);
    this->gameObject.push_back(plane);
    this->gameObject.push_back(smallBunny);
}