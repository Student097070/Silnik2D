#include "Include.h"
#include "Button.h"

Button::Button(float x, float y, float w, float h, string name, ALLEGRO_COLOR ButtonCollor, ALLEGRO_COLOR TextCollor) {}

bool Button:: hovered(float mouseX,float mouseY){
    return (mouseX >= x && x <= x + w && mouseY >= y && y <= y + h);
}

void Button::draw() {
    ALLEGRO_COLOR resolution_color = hovered(ev.mouse.x,ev.mouse.y) ? al_map_rgb(0, 120, 255) : al_map_rgb(0, 0, 0);
    al_draw_filled_rectangle(x, y, x + w, y + h, resolution_color);
    al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(255, 255, 255), 2);
    al_draw_text(fontSmall, al_map_rgb(255, 255, 255), x + (w / 2.0), y + 4,ALLEGRO_ALIGN_CENTRE, name.c_str());
}


