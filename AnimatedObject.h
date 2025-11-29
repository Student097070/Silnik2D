#pragma once
#include "GameObject.h"

class AnimatedObject : virtual public GameObject {
public:
    virtual ~AnimatedObject() = default;
    virtual void animate() = 0;
};