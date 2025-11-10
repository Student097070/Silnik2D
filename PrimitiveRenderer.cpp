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
            dy = y1 - y0;
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
            swap(y0, y1);
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

void PrimitiveRenderer::circlecustom(int x0, int y0, int R) {
    for (float alpha = 0; alpha <= ALLEGRO_PI / 4; alpha += 0.02f) {
        int x = round(R * cos(alpha));
        int y = round(R * sin(alpha));

        al_put_pixel(x0 + x, y0 + y, kolor);
        al_put_pixel(x0 - x, y0 + y, kolor);
        al_put_pixel(x0 + x, y0 - y, kolor);
        al_put_pixel(x0 - x, y0 - y, kolor);
        al_put_pixel(x0 + y, y0 + x, kolor);
        al_put_pixel(x0 - y, y0 + x, kolor);
        al_put_pixel(x0 + y, y0 - x, kolor);
        al_put_pixel(x0 - y, y0 - x, kolor);
    }
}

void PrimitiveRenderer::elipsecustom(int x0, int y0, int Rx, int Ry) {
    for (float alpha = 0; alpha <= ALLEGRO_PI / 4; alpha += 0.02f) {
        int x = round(Rx * cos(alpha));
        int y = round(Ry * sin(alpha));

        al_put_pixel(x0 + x, y0 + y, kolor);
        al_put_pixel(x0 - x, y0 + y, kolor);
        al_put_pixel(x0 + x, y0 - y, kolor);
        al_put_pixel(x0 - x, y0 - y, kolor);
        al_put_pixel(x0 + y * Rx / Ry, y0 + x * Ry / Rx, kolor);
        al_put_pixel(x0 - y * Rx / Ry, y0 + x * Ry / Rx, kolor);
        al_put_pixel(x0 + y * Rx / Ry, y0 - x * Ry / Rx, kolor);
        al_put_pixel(x0 - y * Rx / Ry, y0 - x * Ry / Rx, kolor);
    }
}

void PrimitiveRenderer::polygon(const std::vector<PolygonPoint>& points) {
    if (points.size() < 3) return;

    auto doIntersect = [](PolygonPoint p1, PolygonPoint q1, PolygonPoint p2, PolygonPoint q2) -> bool {
        auto orientation = [](PolygonPoint a, PolygonPoint b, PolygonPoint c) {
            int val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
            if (val == 0) return 0;
            return (val > 0) ? 1 : 2;
            };
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);
        return (o1 != o2 && o3 != o4);
        };

    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            if (doIntersect(points[i], points[(i + 1) % points.size()],
                points[j], points[(j + 1) % points.size()])) {
                //return;
            }
        }
    }

    for (size_t i = 0; i < points.size(); ++i) {
        line(points[i].x, points[i].y, points[(i + 1) % points.size()].x, points[(i + 1) % points.size()].y);
    }
}
