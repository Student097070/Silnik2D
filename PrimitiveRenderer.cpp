#include "Include.h"
#include "PrimitiveRenderer.h"
#include "LineSegment.h"
#include <cmath>
#include <algorithm>
using namespace std;

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
    const float step = 0.02f;
    const float max_alpha = ALLEGRO_PI / 4.0f + step;

    for (float alpha = 0; alpha <= max_alpha; alpha += step) {
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
    const float step = 0.02f;
    const float max_alpha = ALLEGRO_PI / 4.0f + step;
    for (float alpha = 0; alpha <= max_alpha; alpha += step) {
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

void PrimitiveRenderer::polyline(const vector<Point2D>& points) {
    if (points.size() < 2) return;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        line(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }
}

void PrimitiveRenderer::closedPolyline(const vector<Point2D>& points) {
    if (points.size() < 2) return;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        line(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }

    line(points[points.size() - 1].x, points[points.size() - 1].y,
        points[0].x, points[0].y);
}

void PrimitiveRenderer::polylineFromSegments(const vector<LineSegment>& segments) {
    for (const auto& segment : segments) {
        Point2D start = segment.getStart();
        Point2D end = segment.getEnd();
        line(start.x, start.y, end.x, end.y);
    }
}

void PrimitiveRenderer::polygon(const vector<PolygonPoint>& points) {
    if (points.size() < 3) return;

    for (size_t i = 0; i < points.size(); ++i) {
        size_t next_index = (i + 1) % points.size();
        line(points[i].x, points[i].y, points[next_index].x, points[next_index].y);
    }
}

// helper
bool colorsEqual(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2)
{
    unsigned char r1, g1, b1;
    unsigned char r2, g2, b2;
    al_unmap_rgb(c1, &r1, &g1, &b1);
    al_unmap_rgb(c2, &r2, &g2, &b2);
    return (r1 == r2 && g1 == g2 && b1 == b2);
}

// boundaryFill - iteracyjna, bez wyjœcia poza ekran
void PrimitiveRenderer::boundaryFill(int x, int y, ALLEGRO_COLOR fillColor, ALLEGRO_COLOR boundaryColor)
{
    // pobierz backbuffer (bitmapê) i zablokuj
    ALLEGRO_DISPLAY* d = al_get_current_display();
    if (!d) return;
    ALLEGRO_BITMAP* bb = al_get_backbuffer(d);
    if (!bb) return;

    int w = al_get_bitmap_width(bb);
    int h = al_get_bitmap_height(bb);

    // sprawdŸ punkt startowy w obrêbie bitmapy
    if (x < 0 || x >= w || y < 0 || y >= h) return;

    al_lock_bitmap(bb, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);

    // od razu pobierz kolor startowy - jeœli równy fillColor lub boundaryColor -> nic nie robimy
    ALLEGRO_COLOR startColor = al_get_pixel(bb, x, y);
    if (colorsEqual(startColor, fillColor) || colorsEqual(startColor, boundaryColor)) {
        al_unlock_bitmap(bb);
        return;
    }

    stack<pair<int, int>> st;
    st.push({ x,y });

    while (!st.empty()) {
        auto p = st.top(); st.pop();
        int px = p.first, py = p.second;

        // bounds check
        if (px < 0 || px >= w || py < 0 || py >= h) continue;

        ALLEGRO_COLOR cur = al_get_pixel(bb, px, py);
        if (!colorsEqual(cur, fillColor) && !colorsEqual(cur, boundaryColor)) {
            al_put_pixel(px, py, fillColor);

            st.push({ px + 1, py });
            st.push({ px - 1, py });
            st.push({ px, py + 1 });
            st.push({ px, py - 1 });
        }
    }

    al_unlock_bitmap(bb);
}

// floodFill - iteracyjna (wype³nia obszar pixel==backgroundColor)
void PrimitiveRenderer::floodFill(int x, int y, ALLEGRO_COLOR fillColor, ALLEGRO_COLOR backgroundColor)
{
    ALLEGRO_DISPLAY* d = al_get_current_display();
    if (!d) return;
    ALLEGRO_BITMAP* bb = al_get_backbuffer(d);
    if (!bb) return;

    int w = al_get_bitmap_width(bb);
    int h = al_get_bitmap_height(bb);

    if (x < 0 || x >= w || y < 0 || y >= h) return;

    al_lock_bitmap(bb, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);

    ALLEGRO_COLOR startColor = al_get_pixel(bb, x, y);
    // jeœli startColor != backgroundColor lub startColor == fillColor -> nic
    if (!colorsEqual(startColor, backgroundColor) || colorsEqual(startColor, fillColor)) {
        al_unlock_bitmap(bb);
        return;
    }

    stack<pair<int, int>> st;
    st.push({ x,y });

    while (!st.empty()) {
        auto p = st.top(); st.pop();
        int px = p.first, py = p.second;

        if (px < 0 || px >= w || py < 0 || py >= h) continue;

        ALLEGRO_COLOR cur = al_get_pixel(bb, px, py);
        if (colorsEqual(cur, backgroundColor) && !colorsEqual(cur, fillColor)) {
            al_put_pixel(px, py, fillColor);

            st.push({ px + 1, py });
            st.push({ px - 1, py });
            st.push({ px, py + 1 });
            st.push({ px, py - 1 });
        }
    }

    al_unlock_bitmap(bb);
}
