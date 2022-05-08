#include "collisions.h"
#include <iostream>
#include "GameObject.h"
#include "../renderer/BoundingBox.h"

#define DELTA 0.004

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

bool pointBoundingBoxCollision(glm::vec4 point, BoundingBox& bb) {
    bool xAxis = (point.x - bb.max.x) <= DELTA && (point.x - bb.min.x) >= DELTA;
    bool yAxis = (point.y - bb.max.y) <= DELTA && (point.y - bb.min.y) >= DELTA;
    bool zAxis = (point.z - bb.max.z) <= DELTA && (point.z - bb.min.z) >= DELTA;
    //std::cout << "obj coords:   " << point.y << " " << point.x << " " << point.z << std::endl;
    //std::cout << "bb coords: " << bb.min.y << " " << bb.max.y << " " << bb.min.x << " " << bb.max.x << " " << bb.min.z << " " << bb.max.z << std::endl;
    //std::cout << yAxis << " " << xAxis << " " << zAxis << std::endl;
    return yAxis && xAxis && zAxis;
}

bool pointPlaneCollision(glm::vec4 point, BoundingBox& bb) {
    bool yAxis = (point.y - (bb.max.y)) <= DELTA && (point.y - (bb.min.y)) >= -DELTA;
    bool xAxis = (point.x - (bb.max.x)) <= DELTA && (point.x - (bb.min.x)) >= -DELTA;
    bool zAxis = (point.z - (bb.max.z)) <= DELTA && (point.z - (bb.min.z)) >= -DELTA;
    //std::cout << "obj coords:   " << point.y << " " << point.x << " " << point.z << std::endl;
    //std::cout << "bb coords: " << bb.min.y << " " << bb.max.y << " " << bb.min.x << " " << bb.max.x << " " << bb.min.z << " " << bb.max.z << std::endl;
    //std::cout << yAxis << " " << xAxis << " " << zAxis << std::endl;
    return yAxis && xAxis && zAxis;
}

bool boundBoxCollision(BoundingBox& object1, BoundingBox& object2) {
    bool xAxis = object1.min.x <= object2.max.x && object1.max.x >= object2.min.x;
    bool yAxis = object1.min.y <= object2.max.y && object1.max.y >= object2.min.y;
    bool zAxis = object1.min.z <= object2.max.z && object1.max.z >= object2.min.z;

    return yAxis && xAxis && zAxis;
}
