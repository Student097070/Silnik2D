#include "Point2D.h"
#include "Utils.h"  // Dodaj ten include dla logowania

Point2D::Point2D(float x, float y, ALLEGRO_COLOR kolor) {
    this->x = x;
    this->y = y;    
    this->kolor = kolor;
}

// DODAJ IMPLEMENTACJÊ DESTRUKTORA
Point2D::~Point2D() {
    try {
        // Logowanie destrukcji punktu
        Utils::logDebug("Point2D destroyed: (" + to_string(x) + "," + to_string(y) + ")");
    }
    catch (const exception& e) {
        cerr << "Error in Point2D destructor: " << e.what() << endl;
    }
    catch (...) {
        cerr << "Unknown error in Point2D destructor." << endl;
    }
}

void Point2D::ReadPoint() {
    try {
        if (ev.mouse.x == x && ev.mouse.y == y) {
            printf("Point coordinates: (%.2f, %.2f)\n", x, y);
        }
    }
    catch (const exception& e) {
        cerr << "Error reading point: " << e.what() << endl;
    }
}

void Point2D::ChangeCoordinates(float newX, float newY) {
    try {
        x = newX;
        y = newY;
    }
    catch (const exception& e) {
        cerr << "Error changing point coordinates: " << e.what() << endl;
    }
}

void Point2D::DisplayPoint() {
    try {
        al_put_pixel(x, y, kolor);
    }
    catch (const exception& e) {
        cerr << "Error displaying point: " << e.what() << endl;
    }
}