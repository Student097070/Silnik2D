#include "Point2D.h"
#include "PrimitiveRenderer.h"

void Point2D::draw(PrimitiveRenderer& renderer) const {
    renderer.point(x, y);
}