#pragma once
#include "ShapeObject.h"
#include <vector>

// Polygon przechowuje listê wierzcho³ków (PolygonPoint / Point2D)
class ShapePolygon : public ShapeObject {
public:
    ShapePolygon(const std::vector<Point2D>& pts = {}, ALLEGRO_COLOR col = ALLEGRO_COLOR())
    {
        points = pts;
        if (col.a == 0) color = al_map_rgb(0, 0, 0);
        else color = col;
    }

    void draw() override {
        PrimitiveRenderer renderer(color);
        // zak³adam, ¿e PrimitiveRenderer::polygon przyjmuje vector<PolygonPoint>
        // u Ciebie jest typ PolygonPoint — konwertujemy
        std::vector<PolygonPoint> ppts;
        ppts.reserve(points.size());
        for (auto& p : points) {
            PolygonPoint pp; pp.x = (int)round(p.x); pp.y = (int)round(p.y);
            ppts.push_back(pp);
        }
        renderer.polygon(ppts);
    }

    void translate(float tx, float ty) override {
        for (auto& p : points) p.translate(tx, ty);
    }

    void rotate(float alpha, float cx = 0.0f, float cy = 0.0f) override {
        for (auto& p : points) p.rotate(alpha, cx, cy);
    }

    void scale(float kx, float ky, float cx = 0.0f, float cy = 0.0f) override {
        for (auto& p : points) p.scale(kx, ky, cx, cy);
    }

    std::vector<Point2D> points;
};
