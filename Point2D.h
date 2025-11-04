#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <string>

class Point2D {
private:
    int x, y;

public:
    // Konstruktory
    Point2D(int x = 0, int y = 0) : x(x), y(y) {}

    // Gettery
    int getX() const { return x; }
    int getY() const { return y; }

    // Settery
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    void setPosition(int newX, int newY) { x = newX; y = newY; }

    // Rysowanie punktu - deklaracja
    void draw(class PrimitiveRenderer& renderer) const;

    // Przesuniêcie punktu
    void translate(int dx, int dy) {
        x += dx;
        y += dy;
    }

    // Konwersja na string
    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};