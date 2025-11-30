#pragma once
#include "Include.h"
#include "ShapeObject.h"
#include "TransformableObject.h"
#include "STR.h"

class RectangleObject : public ShapeObject, public TransformableObject {
public:
    RectangleData data;
    // Dodajemy zmienne do przechowywania oryginalnych wierzcho³ków
    float originalX0, originalY0, originalX1, originalY1;
    float rotationAngle = 0.0f;

    RectangleObject(const RectangleData& d) : data(d) {
        // Zapisz oryginalne pozycje
        originalX0 = d.x0;
        originalY0 = d.y0;
        originalX1 = d.x1;
        originalY1 = d.y1;
    }

    // ---- RYSOWANIE ----
    void draw() override {
        al_draw_filled_rectangle(data.x0, data.y0, data.x1, data.y1, data.color);
    }

    // ---- TRANSLACJA ----
    void translate(float tx, float ty) override {
        data.x0 += tx; data.y0 += ty;
        data.x1 += tx; data.y1 += ty;
        originalX0 += tx; originalY0 += ty;
        originalX1 += tx; originalY1 += ty;
    }

    // ---- OBRÓT ----
    void rotate(float alpha, float cx, float cy) override {
        rotationAngle += alpha;

        // Przywróæ oryginalne pozycje
        float x0 = originalX0, y0 = originalY0;
        float x1 = originalX1, y1 = originalY1;

        // Obróæ wszystkie 4 wierzcho³ki
        rotatePoint(x0, y0, rotationAngle, cx, cy);
        rotatePoint(x1, y1, rotationAngle, cx, cy);

        // Zaktualizuj dane prostok¹ta
        data.x0 = x0;
        data.y0 = y0;
        data.x1 = x1;
        data.y1 = y1;
    }

    // ---- SKALOWANIE ----
    void scale(float kx, float ky, float cx, float cy) override {
        // Skaluj oryginalne pozycje
        scalePoint(originalX0, originalY0, kx, ky, cx, cy);
        scalePoint(originalX1, originalY1, kx, ky, cx, cy);

        // Zastosuj aktualn¹ rotacjê do przeskalowanych punktów
        float x0 = originalX0, y0 = originalY0;
        float x1 = originalX1, y1 = originalY1;

        rotatePoint(x0, y0, rotationAngle, cx, cy);
        rotatePoint(x1, y1, rotationAngle, cx, cy);

        data.x0 = x0;
        data.y0 = y0;
        data.x1 = x1;
        data.y1 = y1;
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