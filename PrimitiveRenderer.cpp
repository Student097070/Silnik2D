/**
 * @file PrimitiveRenderer.cpp
 * @brief Implementacja klasy PrimitiveRenderer – renderera prymitywów graficznych.
 * @details
 * - Zawiera implementacje metod rysowania kszta³tów podstawowych (prostok¹tów, okrêgów, trójk¹tów).
 * - Implementuje algorytm przyrostowy rysowania linii.
 * - Zawiera niestandardowe algorytmy rysowania okrêgów i elips.
 * - Implementuje algorytmy wype³niania obszarów (boundary fill, flood fill).
 */
#include "Include.h"
#include "PrimitiveRenderer.h"
#include "LineSegment.h"
#include <cmath>
#include <algorithm>
using namespace std;

/**
 * @brief Rysuje prostok¹t (wype³niony lub obramowany).
 * @param x0 Wspó³rzêdna X lewego górnego rogu.
 * @param y0 Wspó³rzêdna Y lewego górnego rogu.
 * @param x1 Wspó³rzêdna X prawego dolnego rogu.
 * @param y1 Wspó³rzêdna Y prawego dolnego rogu.
 * @param wypelniony Czy prostok¹t ma byæ wype³niony.
 * @param grubosc Gruboœæ linii obramowania.
 */
void PrimitiveRenderer::rectangle(int x0, int y0, int x1, int y1, bool wypelniony, float grubosc) {
    if (wypelniony)
        al_draw_filled_rectangle(x0, y0, x1, y1, kolor);
    else
        al_draw_rectangle(x0, y0, x1, y1, kolor, grubosc);
}
/**
 * @brief Rysuje okr¹g (wype³niony lub obramowany).
 * @param x Wspó³rzêdna X œrodka okrêgu.
 * @param y Wspó³rzêdna Y œrodka okrêgu.
 * @param r Promieñ okrêgu.
 * @param wypelniony Czy okr¹g ma byæ wype³niony.
 * @param grubosc Gruboœæ linii obramowania.
 */
void PrimitiveRenderer::circle(int x, int y, int r, bool wypelniony, float grubosc) {
    if (wypelniony)
        al_draw_filled_circle(x, y, r, kolor);
    else
        al_draw_circle(x, y, r, kolor, grubosc);
}
/**
 * @brief Rysuje trójk¹t (wype³niony lub obramowany).
 * @param x0 Wspó³rzêdna X pierwszego wierzcho³ka.
 * @param y0 Wspó³rzêdna Y pierwszego wierzcho³ka.
 * @param x1 Wspó³rzêdna X drugiego wierzcho³ka.
 * @param y1 Wspó³rzêdna Y drugiego wierzcho³ka.
 * @param x2 Wspó³rzêdna X trzeciego wierzcho³ka.
 * @param y2 Wspó³rzêdna Y trzeciego wierzcho³ka.
 * @param wypelniony Czy trójk¹t ma byæ wype³niony.
 * @param grubosc Gruboœæ linii obramowania.
 */
void PrimitiveRenderer::triangle(int x0, int y0, int x1, int y1, int x2, int y2, bool wypelniony, float grubosc) {
    if (wypelniony)
        al_draw_filled_triangle(x0, y0, x1, y1, x2, y2, kolor);
    else
        al_draw_triangle(x0, y0, x1, y1, x2, y2, kolor, grubosc);
}
/**
 * @brief Rysuje liniê algorytmem przyrostowym.
 * @details Implementuje zmodyfikowany algorytm Bresenhama dla wszystkich oktantów.
 * @param x0 Wspó³rzêdna X pocz¹tku linii.
 * @param y0 Wspó³rzêdna Y pocz¹tku linii.
 * @param x1 Wspó³rzêdna X koñca linii.
 * @param y1 Wspó³rzêdna Y koñca linii.
 */
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

/**
 * @brief Rysuje okr¹g niestandardowym algorytmem wykorzystuj¹cym symetriê.
 * @details Rysuje tylko 1/8 okrêgu i wykorzystuje symetriê do narysowania pozosta³ych czêœci.
 * @param x0 Wspó³rzêdna X œrodka okrêgu.
 * @param y0 Wspó³rzêdna Y œrodka okrêgu.
 * @param R Promieñ okrêgu.
 */
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
/**
 * @brief Rysuje elipsê niestandardowym algorytmem.
 * @details Podobny do algorytmu okrêgu, ale z ró¿nymi promieniami dla osi X i Y.
 * @param x0 Wspó³rzêdna X œrodka elipsy.
 * @param y0 Wspó³rzêdna Y œrodka elipsy.
 * @param Rx Promieñ w osi X.
 * @param Ry Promieñ w osi Y.
 */
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
/**
 * @brief Rysuje liniê ³aman¹ (otwart¹).
 * @param points Wektor punktów definiuj¹cych ³aman¹.
 */
void PrimitiveRenderer::polyline(const vector<Point2D>& points) {
    if (points.size() < 2) return;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        line(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }
}
/**
 * @brief Rysuje zamkniêt¹ liniê ³aman¹.
 * @param points Wektor punktów definiuj¹cych ³aman¹.
 */
void PrimitiveRenderer::closedPolyline(const vector<Point2D>& points) {
    if (points.size() < 2) return;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        line(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }

    line(points[points.size() - 1].x, points[points.size() - 1].y,
        points[0].x, points[0].y);
}
/**
 * @brief Rysuje liniê ³aman¹ z segmentów.
 * @param segments Wektor segmentów linii.
 */
void PrimitiveRenderer::polylineFromSegments(const vector<LineSegment>& segments) {
    for (const auto& segment : segments) {
        Point2D start = segment.getStart();
        Point2D end = segment.getEnd();
        line(start.x, start.y, end.x, end.y);
    }
}
/**
 * @brief Rysuje wielok¹t (zamkniêty).
 * @param points Wektor punktów definiuj¹cych wielok¹t.
 */
void PrimitiveRenderer::polygon(const vector<PolygonPoint>& points) {
    if (points.size() < 3) return;

    for (size_t i = 0; i < points.size(); ++i) {
        size_t next_index = (i + 1) % points.size();
        line(points[i].x, points[i].y, points[next_index].x, points[next_index].y);
    }
}

/**
 * @brief Porównuje dwa kolory Allegro.
 * @param c1 Pierwszy kolor.
 * @param c2 Drugi kolor.
 * @return True jeœli kolory s¹ identyczne, false w przeciwnym razie.
 */
bool colorsEqual(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2)
{
    unsigned char r1, g1, b1;
    unsigned char r2, g2, b2;
    al_unmap_rgb(c1, &r1, &g1, &b1);
    al_unmap_rgb(c2, &r2, &g2, &b2);
    return (r1 == r2 && g1 == g2 && b1 == b2);
}

/**
 * @brief Wype³nia obszar algorytmem boundary fill (wype³nianie przez granicê).
 * @details Algorytm iteracyjny wykorzystuj¹cy stos. Wype³nia obszar ograniczony kolorem boundaryColor.
 * @param x Wspó³rzêdna X punktu startowego.
 * @param y Wspó³rzêdna Y punktu startowego.
 * @param fillColor Kolor wype³nienia.
 * @param boundaryColor Kolor granicy obszaru.
 */
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

/**
 * @brief Wype³nia obszar algorytmem flood fill (wype³nianie przez podobny kolor).
 * @details Algorytm iteracyjny wype³niaj¹cy obszar o okreœlonym kolorze t³a.
 * @param x Wspó³rzêdna X punktu startowego.
 * @param y Wspó³rzêdna Y punktu startowego.
 * @param fillColor Kolor wype³nienia.
 * @param backgroundColor Kolor t³a do zast¹pienia.
 */
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
