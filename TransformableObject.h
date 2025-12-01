/**
 * @file TransformableObject.h
 * @brief Nag³ówek interfejsu TransformableObject – obiektów transformowalnych.
 * @details
 * - Interfejs dla obiektów, które mo¿na przesuwaæ, obracaæ i skalowaæ.
 * - Dziedziczy po GameObject.
 */

#pragma once
#include "GameObject.h"
#include "Point2D.h"

 /**
  * @class TransformableObject
  * @brief Interfejs dla obiektów transformowalnych.
  */
class TransformableObject : public virtual GameObject {
public:
    virtual ~TransformableObject() = default;

    /**
     * @brief Translacja obiektu.
     * @param tx Przesuniêcie w osi X.
     * @param ty Przesuniêcie w osi Y.
     */
    virtual void translate(float tx, float ty) = 0;
    /**
 * @brief Obrót obiektu.
 * @param alpha K¹t obrotu w radianach.
 * @param cx Wspó³rzêdna X punktu obrotu.
 * @param cy Wspó³rzêdna Y punktu obrotu.
 */

    virtual void rotate(float alpha, float cx = 0.0f, float cy = 0.0f) = 0;

    /**
     * @brief Skalowanie obiektu.
     * @param kx Wspó³czynnik skali w osi X.
     * @param ky Wspó³czynnik skali w osi Y.
     * @param cx Wspó³rzêdna X punktu skali.
     * @param cy Wspó³rzêdna Y punktu skali.
     */
    virtual void scale(float kx, float ky, float cx = 0.0f, float cy = 0.0f) = 0;
};
