/**
 * @file ShapeCircle.h
 * @brief Nag³ówek klasy ShapeCircle – okrêgu jako obiektu kszta³tu.
 * @details
 * - Dziedziczy po ShapeObject.
 * - Przechowuje œrodek i promieñ okrêgu.
 * - Implementuje transformacje geometryczne z uwzglêdnieniem skalowania promienia.
 */

#pragma once
#include "ShapeObject.h"
#include "Point2D.h"


 /**
  * @class ShapeCircle
  * @brief Klasa reprezentuj¹ca okr¹g.
  */
class ShapeCircle : public ShapeObject {
public:
	ALLEGRO_COLOR c = al_map_rgb(0, 0, 0);
    /**
 * @brief Konstruktor okrêgu.
 * @param cx Wspó³rzêdna X œrodka.
 * @param cy Wspó³rzêdna Y œrodka.
 * @param r Promieñ.
 * @param col Kolor.
 */
    ShapeCircle(float cx = 0, float cy = 0, float r = 10, ALLEGRO_COLOR col = al_map_rgb(0, 0, 0))
        : center(cx, cy,c), radius(r) {
        color = col;
    }

    /**
     * @brief Rysuje okr¹g.
     */
    void draw() override {
        PrimitiveRenderer renderer(color);
        // rysujemy na aktualnym target (renderer korzysta z al_put_pixel lub al_draw_circle)
        renderer.circle((int)round(center.x), (int)round(center.y), (int)round(radius), false, 1.0f);
    }

    // Transformacje
    void translate(float tx, float ty) override {
        center.translate(tx, ty);
    }

    void rotate(float alpha, float cx = 0.0f, float cy = 0.0f) override {
        center.rotate(alpha, cx, cy);

    }

    void scale(float kx, float ky, float cx = 0.0f, float cy = 0.0f) override {
        // przesuwamy œrodek wzglêdem punktu i skalujemy promieñ przy œredniej k
        center.scale(kx, ky, cx, cy);
        // uœrednienie skali promienia (prosty przybli¿ony sposób)
        radius *= 0.5f * (std::abs(kx) + std::abs(ky));
    }

    Point2D center;
    float radius;
};
