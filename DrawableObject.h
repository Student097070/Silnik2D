/**
 * @file DrawableObject.h
 * @brief Nag³ówek interfejsu DrawableObject – obiektów rysowalnych.
 * @details
 * - Interfejs dla wszystkich obiektów, które mog¹ byæ wyœwietlane na ekranie.
 * - Dziedziczy po GameObject.
 */

#pragma once
#include "GameObject.h"

 /**
  * @class DrawableObject
  * @brief Interfejs dla obiektów rysowalnych.
  */

class DrawableObject : public virtual GameObject {
public:
    virtual ~DrawableObject() = default;
    virtual void draw() = 0; ///< Rysuje obiekt
};
