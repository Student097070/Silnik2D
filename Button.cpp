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

bool Button:: hovered(float mouseX,float mouseY){
    return (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h);
}

void Button::draw() {
    ALLEGRO_COLOR FilledColor = hovered(ev.mouse.x, ev.mouse.y) ? al_map_rgb(0, 120, 255) : ButtonCollor;
    al_draw_filled_rectangle(x, y, x + w, y + h, FilledColor);
    al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(255, 255, 255), 2);
    al_draw_text(fontSmall, al_map_rgb(255, 255, 255), x + (w / 2.0), y + 4,ALLEGRO_ALIGN_CENTRE, name.c_str());
}


Button::~Button() {}
