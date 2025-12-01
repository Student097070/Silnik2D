/**
 * @file AnimatedObject.h
 * @brief Nag³ówek interfejsu AnimatedObject – obiektów animowanych.
 * @details
 * - Interfejs dla obiektów, które wymagaj¹ aktualizacji w czasie.
 * - Dziedziczy po GameObject.
 */
#pragma once
#include "GameObject.h"

 /**
  * @class AnimatedObject
  * @brief Interfejs dla obiektów animowanych.
  */
class AnimatedObject : virtual public GameObject {
public:
    virtual ~AnimatedObject() = default;
    virtual void animate(float dt) = 0; ///< Aktualizuje animacjê
};