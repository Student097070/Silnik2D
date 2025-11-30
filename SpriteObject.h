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
    SpriteObject(float x = 0, float y = 0) : BitmapObject() { pos.x; pos.y; }


    void setupSprite(int fw, int fh, int frames, float timePerFrame = 0.1f) {
        frameWidth = fw;
        frameHeight = fh;
        frameCount = frames;
        frameTime = timePerFrame;
    }

    virtual void animate(float dt) override {
        timeAccumulator += dt;

        if (timeAccumulator >= frameTime) {
            currentFrame = (currentFrame + 1) % frameCount;
            timeAccumulator = 0.0f;
        }
    }

    virtual void draw() override {
        if (bitmaps.empty()) return;

        ALLEGRO_BITMAP* sheet = bitmaps[0]->get();
        if (!sheet) return;

        al_draw_bitmap_region(
            sheet,
            currentFrame * frameWidth,
            0,
            frameWidth,
            frameHeight,
            pos.x,
            pos.y,
            0
        );
    }
};
