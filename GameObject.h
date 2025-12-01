/**
 * @file GameObject.h
 * @brief Nag³ówek klasy bazowej GameObject.
 * @details
 * - Najwy¿sza klasa bazowa dla wszystkich obiektów w programie.
 * - Czysto wirtualna, s³u¿y jako interfejs.
 */

#pragma once
 /**
  * @class GameObject
  * @brief Bazowa klasa dla wszystkich obiektów w aplikacji.
  */
class GameObject {
public:
    virtual ~GameObject() = default;
};
