#include "LineSegment.h"
#include "PrimitiveRenderer.h"

LineSegment::LineSegment(Point2D start, Point2D end, ALLEGRO_COLOR col)
    : p1(start), p2(end), color(col) {
}

LineSegment::LineSegment(float x0, float y0, float x1, float y1, ALLEGRO_COLOR col)
    : p1(x0, y0, col), p2(x1, y1, col), color(col) {
}

Point2D LineSegment::getStart() const {
    return p1;
}

Point2D LineSegment::getEnd() const {
    return p2;
}

void LineSegment::setStart(Point2D newStart) {
    p1 = newStart;
}

void LineSegment::setEnd(Point2D newEnd) {
    p2 = newEnd;
}

void LineSegment::setStart(float x, float y) {
    p1.x = x;
    p1.y = y;
}

void LineSegment::setEnd(float x, float y) {
    p2.x = x;
    p2.y = y;
}

void LineSegment::draw(bool useIncrementalAlgorithm) {
    if (useIncrementalAlgorithm) {
        // Algorytm przyrostowy - u¿yj PrimitiveRenderer
        PrimitiveRenderer renderer(color);
        renderer.line(p1.x, p1.y, p2.x, p2.y);
    }
    else {
        // Domyœlny algorytm z Allegro
        al_draw_line(p1.x, p1.y, p2.x, p2.y, color, 1.0f);
    }
}

LineSegment::~LineSegment() {}