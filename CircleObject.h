#pragma once
#include "Include.h"
#include "ShapeObject.h"
#include "TransformableObject.h"
#include "STR.h"

class CircleObject : public ShapeObject, public TransformableObject {
public:
    Circle2Data data;

    CircleObject(float x, float y, float r, ALLEGRO_COLOR color) {
        data.x0 = x;
        data.y0 = y;
        data.R = r;
        data.color = color;
    }

    // ---- RYSOWANIE ----
    void draw() override {
        al_draw_filled_circle(data.x0, data.y0, data.R, data.color);
    }

    // ---- TRANSLACJA ----
    void translate(float tx, float ty) override {
        data.x0 += tx;
        data.y0 += ty;
    }

    // ---- OBRÓT ----
    void rotate(float alpha, float cx, float cy) override {
        rotatePoint(data.x0, data.y0, alpha, cx, cy);
    }

    // ---- SKALOWANIE ----
    void scale(float kx, float ky, float cx, float cy) override {
        // Dla ko³a u¿ywamy œredniej z kx i ky aby zachowaæ okr¹g³oœæ
        float k = (kx + ky) / 2.0f;

        // Skalowanie pozycji œrodka
        scalePoint(data.x0, data.y0, kx, ky, cx, cy);

        // Skalowanie promienia
        data.R *= k;
    }

    // ---- CENTRUM KO£A ----
    void getCenter(float& cx, float& cy) {
        cx = data.x0;
        cy = data.y0;
    }

private:
    void rotatePoint(float& x, float& y, float alpha, float cx, float cy) {
        float s = sin(alpha);
        float c = cos(alpha);
        x -= cx; y -= cy;
        float nx = x * c - y * s;
        float ny = x * s + y * c;
        x = nx + cx;
        y = ny + cy;
    }

    void scalePoint(float& x, float& y, float kx, float ky, float cx, float cy) {
        x = cx + (x - cx) * kx;
        y = cy + (y - cy) * ky;
    }
};