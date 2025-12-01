/**
 * @file BitmapHandler.h
 * @brief Nag³ówek klasy BitmapHandler – zarz¹dzania bitmapami Allegro.
 * @details
 * - Opakowuje ALLEGRO_BITMAP, zarz¹dza cyklem ¿ycia.
 * - Udostêpnia metody ³adowania, zapisywania i rysowania bitmap.
 */
#pragma once
#include "Include.h"

 /**
  * @class BitmapHandler
  * @brief Klasa opakowuj¹ca bitmapy Allegro.
  */

class BitmapHandler {
private:
    ALLEGRO_BITMAP* bitmap = nullptr;

public:
    BitmapHandler() = default;
    ~BitmapHandler();

    bool create(int w, int h);
    bool loadFromFile(const string& filename);
    bool saveToFile(const string& filename);

    void destroy();
    void drawTo(BitmapHandler& target, float x, float y);
    void drawRegionTo(BitmapHandler& target, float sx, float sy, float sw, float sh, float dx, float dy);

    ALLEGRO_BITMAP* get() const { return bitmap; } ///< Zwraca wskaŸnik na bitmapê
};
