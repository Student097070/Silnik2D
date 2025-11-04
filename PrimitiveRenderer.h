#pragma once
#include "Include.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

class PrimitiveRenderer {
private:
    ALLEGRO_COLOR kolor;
public:
    PrimitiveRenderer(ALLEGRO_COLOR kolor = al_map_rgb(0, 0, 0)) : kolor(kolor) {}
    void point(int x, int y);
    void rectangle(int x0, int y0, int x1, int y1, bool wypelniony = false, float grubosc = 1.0f);
    void circle(int x, int y, int r, bool wypelniony = false, float grubosc = 1.0f);
    void triangle(int x0, int y0, int x1, int y1, int x2, int y2, bool wypelniony = false, float grubosc = 1.0f);
    void line(int x0, int y0, int x1, int y1);
};