#pragma once

#include "GameObject.h"
#include "../renderer/BoundingBox.h"


bool boundingBoxCollision(GameObject& first, GameObject& second);

bool boundBoxPlaneCollision(BoundingBox& object, BoundingBox& plane);
bool boundBoxCollision(BoundingBox& object1, BoundingBox& object2);

bool pointBoundingBoxCollision(glm::vec4 point, BoundingBox& bb);
bool pointPlaneCollision(glm::vec4 point, BoundingBox& bb);

