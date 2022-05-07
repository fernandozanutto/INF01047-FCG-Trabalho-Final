#pragma once 

#include <glm/vec4.hpp>

#include "GameObject.h"


class BezierObject : public GameObject {
private:
    float lastUpdate;
    float startTime;
public:
    glm::vec4 p1;
    glm::vec4 p2;
    glm::vec4 p3;
    glm::vec4 p4;
    /**
     * @brief in seconds
     * 
     */
    float movePeriod;
    BezierObject(Model* model, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4, float period, ObjectType type = ObjectType::Comum);
    bool isMoving = true;
    virtual void update() override;
};