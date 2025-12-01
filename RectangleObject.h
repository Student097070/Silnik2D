/**
 * @file RectangleObject.h
 * @brief Nag³ówek klasy RectangleObject – prostok¹ta jako obiektu kszta³tu.
 * @details
 * - Dziedziczy po ShapeObject i TransformableObject.
 * - Przechowuje oryginalne i transformowane wierzcho³ki.
 * - Obs³uguje z³o¿one transformacje z zachowaniem oryginalnych wspó³rzêdnych.
 */
#pragma once
#include "Include.h"
#include "ShapeObject.h"
#include "TransformableObject.h"
#include "STR.h"

 /**
  * @class RectangleObject
  * @brief Klasa reprezentuj¹ca prostok¹t.
  */

class RectangleObject : public ShapeObject, public TransformableObject {
public:
    RectangleData data; ///< Aktualne dane prostok¹ta
    float originalX0, originalY0, originalX1, originalY1;
    float rotationAngle = 0.0f;    ///< Aktualny k¹t obrotu

    /**
   * @brief Konstruktor prostok¹ta.
   * @param d Dane prostok¹ta.
   */
    RectangleObject(const RectangleData& d) : data(d) {

        originalX0 = d.x0;
        originalY0 = d.y0;
        originalX1 = d.x1;
        originalY1 = d.y1;
    }
    /**
     * @brief Rysuje wype³niony prostok¹t.
     */
    void draw() override {
        al_draw_filled_rectangle(data.x0, data.y0, data.x1, data.y1, data.color);
    }

    void translate(float tx, float ty) override {
        data.x0 += tx; data.y0 += ty;
        data.x1 += tx; data.y1 += ty;
        originalX0 += tx; originalY0 += ty;
        originalX1 += tx; originalY1 += ty;
    }

    void rotate(float alpha, float cx, float cy) override {
        rotationAngle += alpha;


        float x0 = originalX0, y0 = originalY0;
        float x1 = originalX1, y1 = originalY1;

  
        rotatePoint(x0, y0, rotationAngle, cx, cy);
        rotatePoint(x1, y1, rotationAngle, cx, cy);

 
        data.x0 = x0;
        data.y0 = y0;
        data.x1 = x1;
        data.y1 = y1;
    }


    void scale(float kx, float ky, float cx, float cy) override {

        scalePoint(originalX0, originalY0, kx, ky, cx, cy);
        scalePoint(originalX1, originalY1, kx, ky, cx, cy);


        float x0 = originalX0, y0 = originalY0;
        float x1 = originalX1, y1 = originalY1;

        rotatePoint(x0, y0, rotationAngle, cx, cy);
        rotatePoint(x1, y1, rotationAngle, cx, cy);

        data.x0 = x0;
        data.y0 = y0;
        data.x1 = x1;
        data.y1 = y1;
    }


    /**
     * @brief Oblicza œrodek prostok¹ta.
     * @param cx Referencja do wspó³rzêdnej X œrodka.
     * @param cy Referencja do wspó³rzêdnej Y œrodka.
     */
    void getCenter(float& cx, float& cy) {
        cx = (data.x0 + data.x1) / 2.0f;
        cy = (data.y0 + data.y1) / 2.0f;
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