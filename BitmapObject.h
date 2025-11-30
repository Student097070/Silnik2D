#pragma once
#include <vector>
#include <memory>
#include "DrawableObject.h"
#include "TransformableObject.h"
#include "BitmapHandler.h"
#include "Point2D.h"

class BitmapObject : public DrawableObject, public TransformableObject {
protected:
    Point2D pos;   
    float angle = 0;
    float scaleX = 1.0f;
    float scaleY = 1.0f;

    vector<BitmapHandler*> bitmaps;

public:
    BitmapObject() = default;
    /*BitmapObject() {
        pos.x = 0;
        pos.y = 0;
    }*/

    virtual ~BitmapObject() = default;

    // dodawanie bitmap (np. warstw)
    void addBitmap(BitmapHandler* bmp) {
        bitmaps.push_back(bmp);
    }

    // --- TransformableObject ---

    void translate(float tx, float ty) override {
        pos.translate(tx, ty);
    }

    void rotate(float alpha, float cx = 0, float cy = 0) override {
        angle += alpha;
    }

    void scale(float kx, float ky, float cx = 0, float cy = 0) override {
        scaleX *= kx;
        scaleY *= ky;
    }

    // --- DrawableObject ---
    virtual void draw() override {
        for (auto& bmp : bitmaps) {
            ALLEGRO_BITMAP* bitmap = bmp->get();
            if (!bitmap) continue;

            int w = al_get_bitmap_width(bitmap);
            int h = al_get_bitmap_height(bitmap);

            al_draw_scaled_rotated_bitmap(
                bitmap,
                w / 2.0, h / 2.0,   // punkt obrotu w œrodku bitmapy
                pos.x, pos.y,       // pozycja bitmapy
                scaleX, scaleY,     // skalowanie
                angle,              // obrót
                0                   // flagi
            );
        }
    }

};
