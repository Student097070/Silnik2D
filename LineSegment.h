#pragma once
#include "Include.h"
#include "Point2D.h"

// Forward declaration
class PrimitiveRenderer;

class LineSegment {
private:
    Point2D p1, p2;
    ALLEGRO_COLOR color;

public:
    // Konstruktory
    LineSegment(Point2D start, Point2D end, ALLEGRO_COLOR col = al_map_rgb(255, 255, 255));
    LineSegment(float x0, float y0, float x1, float y1, ALLEGRO_COLOR col = al_map_rgb(255, 255, 255));

    // Odczyt wspó³rzêdnych koñców
    Point2D getStart() const;
    Point2D getEnd() const;

    // Modyfikacja wspó³rzêdnych koñców
    void setStart(Point2D newStart);
    void setEnd(Point2D newEnd);
    void setStart(float x, float y);
    void setEnd(float x, float y);

    // Rysowanie odcinka (z wyborem algorytmu)
    void draw(bool useIncrementalAlgorithm = false);
    void translate(float tx, float ty) { p1.translate(tx, ty); p1.translate(tx, ty); }
    void rotate(float alpha, float cx = 0.0f, float cy = 0.0f) { p1.rotate(alpha, cx, cy); p2.rotate(alpha, cx, cy); }
    void scale(float kx, float ky, float cx = 0.0f, float cy = 0.0f) { p1.scale(kx, ky, cx, cy); p2.scale(kx, ky, cx, cy); }
    ~LineSegment();
};