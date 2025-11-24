#pragma once
#include "ShapeObject.h"
#include "Point2D.h"

// Prosty kszta³t - okr¹g przechowuje œrodek i promieñ
class ShapeCircle : public ShapeObject {
public:
    ShapeCircle(float cx = 0, float cy = 0, float r = 10, ALLEGRO_COLOR col = al_map_rgb(0, 0, 0))
        : center(cx, cy), radius(r) {
        color = col;
    }

    // Drawable
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
        // obrót nie zmienia promienia
    }

    void scale(float kx, float ky, float cx = 0.0f, float cy = 0.0f) override {
        // przesuwamy œrodek wzglêdem punktu i skalujemy promieñ przy œredniej k
        center.scale(kx, ky, cx, cy);
        // uœrednienie skali promienia (prosty przybli¿ony sposób)
        radius *= 0.5f * (std::abs(kx) + std::abs(ky));
    }

    // dostêp
    Point2D center;
    float radius;
};
