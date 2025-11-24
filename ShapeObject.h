#pragma once
#include "DrawableObject.h"
#include "TransformableObject.h"
#include "PrimitiveRenderer.h"

// ShapeObject: rysowalny i transformowalny obiekt prymitywny
class ShapeObject : public DrawableObject, public TransformableObject {
public:
    virtual ~ShapeObject() = default;

    // przydatne: kolor prymitywu
    virtual void setColor(ALLEGRO_COLOR c) { color = c; }
    virtual ALLEGRO_COLOR getColor() const { return color; }

protected:
    ALLEGRO_COLOR color = al_map_rgb(0, 0, 0);
};
