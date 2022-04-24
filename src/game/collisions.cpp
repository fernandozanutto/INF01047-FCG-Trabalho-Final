#include "collisions.h"
#include <iostream>
#include "GameObject.h"
#include "../renderer/BoundingBox.h"


bool boundingBoxCollision(GameObject& first, GameObject& second) {
    return false;
}

bool boundBoxPlaneCollision(BoundingBox& object, BoundingBox& plane) {
    bool xAxis = object.min.x <= plane.max.x && object.max.x >= plane.min.x;
    bool yAxis = object.min.y <= plane.max.y && object.max.y >= plane.min.y;
    bool zAxis = object.min.z <= plane.max.z && object.max.z >= plane.min.z;
    //std::cout << "obj coords:   " << object.min.y << " " << object.max.y << " " << object.min.x << " " << object.max.x << " " << object.min.z << " " << object.max.z << std::endl;
    //std::cout << "plane coords: " << plane.min.y << " " << plane.max.y << " " << plane.min.x << " " << plane.max.x << " " << plane.min.z << " " << plane.max.z << std::endl;
    //std::cout << yAxis << " " << xAxis << " " << zAxis << std::endl;
    return yAxis && xAxis && zAxis;
}