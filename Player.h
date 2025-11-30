#pragma once
#include "SpriteObject.h"
#include "UpdatableObject.h"
#include "Include.h"

class Player : public SpriteObject, public UpdatableObject {
public:
    float speed;

    Player(float x = 400, float y = 300, float speed = 200.0f)
        : SpriteObject(), UpdatableObject(), speed(speed)
    {
        pos.x = x;
        pos.y = y;

        // Wczytanie sprite sheet
        auto bmp = new BitmapHandler();
        bmp->loadFromFile("sprity/Gangster/przod/Gangster_00_1.png");
        addBitmap(bmp);

        int fw = al_get_bitmap_width(bmp->get()) / 4;
        int fh = al_get_bitmap_height(bmp->get());

        setupSprite(fw, fh, 4, 0.12f);
    }

    void update(float dt) override {
        ALLEGRO_KEYBOARD_STATE ks;
        al_get_keyboard_state(&ks);

        float dx = 0;
        float dy = 0;

        if (al_key_down(&ks, ALLEGRO_KEY_UP)) dy -= 1;
        if (al_key_down(&ks, ALLEGRO_KEY_DOWN)) dy += 1;
        if (al_key_down(&ks, ALLEGRO_KEY_LEFT)) dx -= 1;
        if (al_key_down(&ks, ALLEGRO_KEY_RIGHT)) dx += 1;

        if (dx != 0 || dy != 0) {
            float len = sqrt(dx * dx + dy * dy);
            dx /= len;
            dy /= len;

            translate(dx * speed * dt, dy * speed * dt);
            animate(dt);
        }
    }
};