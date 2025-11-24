#pragma once
#include "GameObject.h"
#include "Point2D.h"

// interfejs dla obiektów poddawanych transformacjom
class TransformableObject : public virtual GameObject {
public:
    virtual ~TransformableObject() = default;

    // translacja
    virtual void translate(float tx, float ty) = 0;

    // obrót o k¹t alfa (radiany) wzglêdem punktu (cx,cy)
    virtual void rotate(float alpha, float cx = 0.0f, float cy = 0.0f) = 0;

    // skalowanie wzglêdem punktu (cx,cy), kx,ky - wspó³czynniki
    virtual void scale(float kx, float ky, float cx = 0.0f, float cy = 0.0f) = 0;
};
