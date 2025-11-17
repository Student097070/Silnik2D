#pragma once
#include "Include.h"
#include "Point2D.h"
#include "STR.h"

// Forward declaration zamiast include
class LineSegment;

class PrimitiveRenderer {

private:
    ALLEGRO_COLOR kolor;
public:
    PrimitiveRenderer(ALLEGRO_COLOR kolor = al_map_rgb(0, 0, 0)) : kolor(kolor) {}

    void rectangle(int x0, int y0, int x1, int y1, bool wypelniony = false, float grubosc = 1.0f);
    void circle(int x, int y, int r, bool wypelniony = false, float grubosc = 1.0f);
    void triangle(int x0, int y0, int x1, int y1, int x2, int y2, bool wypelniony = false, float grubosc = 1.0f);
    void line(int x0, int y0, int x1, int y1);
    void circlecustom(int x0, int y0, int R);
    void elipsecustom(int x0, int y0, int Rx, int Ry);
    void polygon(const vector<PolygonPoint>& points);

    // Nowe metody dla linii ³amanych
    void polyline(const vector<Point2D>& points);
    void closedPolyline(const vector<Point2D>& points);
    void polylineFromSegments(const vector<LineSegment>& segments);

    virtual ~PrimitiveRenderer() = default;
};