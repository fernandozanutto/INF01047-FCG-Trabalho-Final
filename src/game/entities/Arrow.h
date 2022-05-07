#pragma once

#include "../GameObject.h"

class Arrow : public GameObject {
public:
    Arrow();
    void update() override;
};