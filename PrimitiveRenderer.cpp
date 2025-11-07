#include "Include.h"
#include "PrimitiveRenderer.h"


void PrimitiveRenderer::rectangle(int x0, int y0, int x1, int y1, bool wypelniony, float grubosc) {
    if (wypelniony)
        al_draw_filled_rectangle(x0, y0, x1, y1, kolor);
    else
        al_draw_rectangle(x0, y0, x1, y1, kolor, grubosc);
}
void PrimitiveRenderer::circle(int x, int y, int r, bool wypelniony, float grubosc) {
    if (wypelniony)
        al_draw_filled_circle(x, y, r, kolor);
    else
        al_draw_circle(x, y, r, kolor, grubosc);
}

void PrimitiveRenderer::triangle(int x0, int y0, int x1, int y1, int x2, int y2, bool wypelniony, float grubosc) {
    if (wypelniony)
        al_draw_filled_triangle(x0, y0, x1, y1, x2, y2, kolor);
    else
        al_draw_triangle(x0, y0, x1, y1, x2, y2, kolor, grubosc);
}


void PrimitiveRenderer::line(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    if (abs(dy) <= abs(dx)) {
        if (x0 > x1) {
            swap(x0, x1);
            swap(y0, y1);
            dx = x1 - x0;
            dy = y1 - x0;
        }
        float m = (float)dy / dx;
        float y = y0;
        for (int x = x0; x <= x1; x++) {
            al_put_pixel(x, round(y), kolor);
            y += m;
        }
    }
    else {
        if (y0 > y1) {
            swap(x0, x1);
            swap(y1, y0);
            dx = x1 - x0;
            dy = y1 - y0;

        }

        float m = (float)dx / dy;
        float x = x0;

        for (int y = y0; y <= y1; y++) {
            al_put_pixel(round(x), y, kolor);
            x += m;
        }
    }
}