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