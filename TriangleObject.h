/**
 * @file TriangleObject.h
 * @brief Nag³ówek klasy TriangleObject – trójk¹ta jako obiektu kszta³tu.
 * @details
 * - Dziedziczy po ShapeObject i TransformableObject.
 * - Przechowuje trzy wierzcho³ki trójk¹ta.
 * - Implementuje transformacje geometryczne.
 */

#pragma once
#include "Include.h"
#include "ShapeObject.h"
#include "TransformableObject.h"
#include "STR.h"

 /**
  * @class TriangleObject
  * @brief Klasa reprezentuj¹ca trójk¹t.
  */

class TriangleObject : public ShapeObject, public TransformableObject {
public:
    TriangleData data;

    /**
     * @brief Konstruktor trójk¹ta.
     * @param d Dane trójk¹ta.
     */
    TriangleObject(const TriangleData& d) : data(d) {}

    /**
     * @brief Rysuje wype³niony trójk¹t.
     */
    void draw() override {
        al_draw_filled_triangle(
            data.x0, data.y0,
            data.x1, data.y1,
            data.x2, data.y2,
            data.color
        );
    }

    /**
 * @brief TRANSLACJA
 */
    void translate(float tx, float ty) override {
        data.x0 += tx; data.y0 += ty;
        data.x1 += tx; data.y1 += ty;
        data.x2 += tx; data.y2 += ty;
    }

    /**
* @brief OBRÓT
*/
    void rotate(float alpha, float cx, float cy) override {
        rotatePoint(data.x0, data.y0, alpha, cx, cy);
        rotatePoint(data.x1, data.y1, alpha, cx, cy);
        rotatePoint(data.x2, data.y2, alpha, cx, cy);
    }
    /**
* @brief SKALOWANIE
*/

    void scale(float kx, float ky, float cx, float cy) override {
        scalePoint(data.x0, data.y0, kx, ky, cx, cy);
        scalePoint(data.x1, data.y1, kx, ky, cx, cy);
        scalePoint(data.x2, data.y2, kx, ky, cx, cy);
    }

    /**
* @brief CENTRUM TRÓJK¥TA 
*/

    void getCenter(float& cx, float& cy) {
        cx = (data.x0 + data.x1 + data.x2) / 3.0f;
        cy = (data.y0 + data.y1 + data.y2) / 3.0f;
    }

private:
    void rotatePoint(float& x, float& y, float alpha, float cx, float cy) {
        float s = sin(alpha);
        float c = cos(alpha);
        x -= cx; y -= cy;
        float nx = x * c - y * s;
        float ny = x * s + y * c;
        x = nx + cx;
        y = ny + cy;
    }

    void scalePoint(float& x, float& y, float kx, float ky, float cx, float cy) {
        x = cx + (x - cx) * kx;
        y = cy + (y - cy) * ky;
    }
};
