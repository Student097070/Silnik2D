#pragma once
#include "BitmapObject.h"
#include "AnimatedObject.h"

class SpriteObject : public BitmapObject, public AnimatedObject {
protected:
    int frameWidth = 0;
    int frameHeight = 0;
    int frameCount = 1;
    int currentFrame = 0;
    float frameTime = 0.1f;
    float timeAccumulator = 0.0f;

public:
    SpriteObject(float x = 0, float y = 0) : BitmapObject() { pos.x = x; pos.y = y; }


    void setupSprite(int fw, int fh, int frames, float timePerFrame = 0.1f) {
        frameWidth = fw;
        frameHeight = fh;
        frameCount = frames;
        frameTime = timePerFrame;
    }

    virtual void animate(float dt) override {
        if (bitmaps.empty()) return;

        timeAccumulator += dt;
        if (timeAccumulator >= frameTime) {
            currentFrame = (currentFrame + 1) % bitmaps.size(); // <- bez frameCount
            timeAccumulator = 0.0f;
        }
    }


    virtual void draw() override {
        if (bitmaps.empty()) return;

        ALLEGRO_BITMAP* bmp = bitmaps[currentFrame]->get();
        if (!bmp) return;

        al_draw_bitmap(bmp, pos.x, pos.y, 0);
    }


};
