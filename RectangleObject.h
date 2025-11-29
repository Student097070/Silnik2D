#pragma once
#include "Include.h"
#include "ShapeObject.h"
#include "TransformableObject.h"
#include "STR.h"

class RectangleObject : public ShapeObject, public TransformableObject {
public:
    RectangleData data;

    RectangleObject(const RectangleData& d) : data(d) {}

    // ---- RYSOWANIE ----
    void draw() override {
        al_draw_filled_rectangle(data.x0, data.y0, data.x1, data.y1, data.color);
    }

    // ---- TRANSLACJA ----
    void translate(float tx, float ty) override {
        data.x0 += tx; data.y0 += ty;
        data.x1 += tx; data.y1 += ty;
    }

    // ---- OBRÓT ----
    void rotate(float alpha, float cx, float cy) override {
        rotatePoint(data.x0, data.y0, alpha, cx, cy);
        rotatePoint(data.x1, data.y1, alpha, cx, cy);
    }

    // ---- SKALOWANIE ----
    void scale(float kx, float ky, float cx, float cy) override {
        scalePoint(data.x0, data.y0, kx, ky, cx, cy);
        scalePoint(data.x1, data.y1, kx, ky, cx, cy);
    }

    // ---- CENTRUM PROSTOK¥TA ----
    void getCenter(float& cx, float& cy) {
        cx = (data.x0 + data.x1) / 2.0f;
        cy = (data.y0 + data.y1) / 2.0f;
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