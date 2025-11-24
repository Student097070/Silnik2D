#pragma once
#include "GameObject.h"

// interfejs dla obiektów aktualizowalnych (logika)
class UpdatableObject : public virtual GameObject {
public:
    virtual ~UpdatableObject() = default;
    virtual void update(float dt) = 0; // dt - delta time (sekundy)
};
