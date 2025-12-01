/**
 * @file UpdatableObject.h
 * @brief Nag³ówek interfejsu UpdatableObject – obiektów aktualizowalnych.
 * @details
 * - Interfejs dla obiektów wymagaj¹cych aktualizacji w czasie (np. logika gry).
 * - Dziedziczy po GameObject.
 */
#pragma once
#include "GameObject.h"

 /**
  * @class UpdatableObject
  * @brief Interfejs dla obiektów aktualizowalnych.
  */
class UpdatableObject : public virtual GameObject {
public:
    virtual ~UpdatableObject() = default;
    /**
   * @brief Aktualizuje obiekt.
   * @param dt Czas od ostatniej aktualizacji (delta time) w sekundach.
   */
    virtual void update(float dt) = 0; 
};
