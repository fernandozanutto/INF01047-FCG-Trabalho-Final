#include "GameObject.h"
#include "../renderer/BoundingBox.h"


bool boundingBoxCollision(GameObject& first, GameObject& second);

bool boundBoxPlaneCollision(BoundingBox& object, BoundingBox& plane);