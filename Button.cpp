/**
 * @file Button.cpp
 * @brief Implementacja klasy Button.
 * @details
 * - Rysuje przycisk z obs³ug¹ stanu najechania.
 * - Loguje destrukcjê przycisku do pliku debug.
 */

#include "Include.h"
#include "Button.h"

Button::Button(float x, float y, float w, float h, string name, ALLEGRO_COLOR ButtonColor, ALLEGRO_COLOR TextColor)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->name = name;
    this->ButtonCollor = ButtonColor;
    this->TextCollor = TextColor;
}

bool Button::hovered(float mouseX, float mouseY) {
    if (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h) return true;
    else return false;
}

void Button::draw() {
    ALLEGRO_COLOR FilledColor = hovered(ev.mouse.x, ev.mouse.y) ? al_map_rgb(0, 120, 255) : ButtonCollor;
    al_draw_filled_rectangle(x, y, x + w, y + h, FilledColor);
    al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(255, 255, 255), 2);
    al_draw_text(fontSmall, al_map_rgb(255, 255, 255), x + (w / 2.0), y + 4, ALLEGRO_ALIGN_CENTRE, name.c_str());
}

Button::~Button() {
    try {
        // Logowanie destrukcji przycisku
        ofstream logFile("debug_log.txt", ios::app);
        if (logFile.is_open()) {
            logFile << "Button '" << name << "' destroyed successfully." << endl;
            logFile.close();
        }
    }
    catch (const exception& e) {
        // W przypadku b³êdu logowania, wyœwietl komunikat
        cerr << "Error in Button destructor: " << e.what() << endl;
    }
    catch (...) {
        cerr << "Unknown error in Button destructor." << endl;
    }
}