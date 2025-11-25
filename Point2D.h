#pragma once
#include "Include.h"

class Point2D
{
public:
    float x, y; //wspolrzedne punktu 
    ALLEGRO_COLOR kolor; //kolor punktu

    
    Point2D(float x, float y, ALLEGRO_COLOR kolor);
    ~Point2D();

    void ReadPoint();
    void ChangeCoordinates(float newX, float newY);
    void DisplayPoint();
    // transformacje
    void translate(float tx, float ty) { x += tx; y += ty; }

    // rotate alpha radians around point (cx, cy)
    void rotate(float alpha, float cx = 0.0f, float cy = 0.0f) {
        float sx = x - cx;
        float sy = y - cy;
        float ca = cos(alpha);
        float sa = sin(alpha);
        float nx = sx * ca - sy * sa;
        float ny = sx * sa + sy * ca;
        x = nx + cx;
        y = ny + cy;
    }

    // scale by kx, ky about (cx, cy)
    void scale(float kx, float ky, float cx = 0.0f, float cy = 0.0f) {
        x = cx + (x - cx) * kx;
        y = cy + (y - cy) * ky;
    }
};