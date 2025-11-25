#pragma once
#include "UpdatableObject.h"
#include "DrawableObject.h"
#include "Point2D.h"
#include "PrimitiveRenderer.h"

class Player : public UpdatableObject, public DrawableObject {
public:
    
	ALLEGRO_COLOR c = al_map_rgb(255, 255, 0);
    Point2D pos;
    Player(float x = 400, float y = 300, float speed = 200.0f)
        : pos(x, y, c), speed(speed) {
    }

    void update(float dt) override {
        // prosty ruch klawiszami WSAD
        if (al_key_down) { 
           /* if (ev.keyboard.keycode == ALLEGRO_KEY_W){
            }*/
        
        }
        // tutaj: przyk³adowo - poruszanie zale¿ne od globalnych flag klawiatury
    }

    // szybkie API do zewnêtrznego sterowania
    void moveBy(float dx, float dy) { pos.translate(dx, dy); }

    void draw() override {
        PrimitiveRenderer r(al_map_rgb(255, 255, 0));
        // narysuj ma³y trójk¹t jako gracza
        int x = (int)round(pos.x);
        int y = (int)round(pos.y);
        r.triangle(x, y - 10, x - 8, y + 6, x + 8, y + 6, true, 1.0f);
    }

    
    float speed;
};
