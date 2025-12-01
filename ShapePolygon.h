/**
 * @file ShapePolygon.h
 * @brief Nag³ówek klasy ShapePolygon – wielok¹ta jako obiektu kszta³tu.
 * @details
 * - Dziedziczy po ShapeObject.
 * - Przechowuje listê wierzcho³ków (Point2D).
 * - Implementuje transformacje geometryczne dla ca³ego wielok¹ta.
 */

#pragma once
#include "ShapeObject.h"
#include <vector>

 /**
  * @class ShapePolygon
  * @brief Klasa reprezentuj¹ca wielok¹t.
  */
class ShapePolygon : public ShapeObject {
public:
    /**
    * @brief Konstruktor wielok¹ta.
    * @param pts Lista wierzcho³ków.
    * @param col Kolor wielok¹ta.
    */
    ShapePolygon(const std::vector<Point2D>& pts = {}, ALLEGRO_COLOR col = ALLEGRO_COLOR())
    {
        points = pts;
        if (col.a == 0) color = al_map_rgb(0, 0, 0);
        else color = col;
    }
    /**
  * @brief Rysuje wielok¹t.
  */

    void draw() override {
        PrimitiveRenderer renderer(color);
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
