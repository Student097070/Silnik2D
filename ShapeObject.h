/**
 * @file ShapeObject.h
 * @brief Nag³ówek klasy ShapeObject – bazowej dla obiektów kszta³tów.
 * @details
 * - Dziedziczy po DrawableObject i TransformableObject.
 * - £¹czy mo¿liwoœæ rysowania i transformacji.
 * - Zawiera wspóln¹ w³aœciwoœæ koloru.
 */
#pragma once
#include "DrawableObject.h"
#include "TransformableObject.h"
#include "PrimitiveRenderer.h"
 /**
  * @class ShapeObject
  * @brief Bazowa klasa dla obiektów kszta³tów geometrycznych.
  */
class ShapeObject : public DrawableObject, public TransformableObject {
public:
    virtual ~ShapeObject() = default;

    /**
  * @brief Ustawia kolor kszta³tu.
  * @param c Nowy kolor.
  */
    virtual void setColor(ALLEGRO_COLOR c) { color = c; }

    /**
     * @brief Zwraca aktualny kolor kszta³tu.
     * @return Aktualny kolor.
     */
    virtual ALLEGRO_COLOR getColor() const { return color; }

protected:
    ALLEGRO_COLOR color = al_map_rgb(0, 0, 0);  ///< Kolor kszta³tu
};
