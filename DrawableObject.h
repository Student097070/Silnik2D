#pragma once
#include "GameObject.h"

// interfejs dla rysowalnych obiektów
class DrawableObject : public virtual GameObject {
public:
    virtual ~DrawableObject() = default;
    virtual void draw() = 0;
};
