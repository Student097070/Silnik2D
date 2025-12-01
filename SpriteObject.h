/**
 * @file SpriteObject.h
 * @brief Nag³ówek klasy SpriteObject – animowanego sprite'a.
 * @details
 * - Dziedziczy po BitmapObject i AnimatedObject.
 * - Obs³uguje animacjê sprite'ów z klatkami.
 * - Umo¿liwia konfiguracjê wymiarów klatek i czasu animacji.
 */
#pragma once
#include "BitmapObject.h"
#include "AnimatedObject.h"

 /**
  * @class SpriteObject
  * @brief Klasa reprezentuj¹ca animowanego sprite'a.
  */

class SpriteObject : public BitmapObject, public AnimatedObject {
protected:
    int frameWidth = 0;         ///< Szerokoœæ pojedynczej klatki
    int frameHeight = 0;        ///< Wysokoœæ pojedynczej klatki
    int frameCount = 1;         ///< Liczba klatek (nieu¿ywane)
    int currentFrame = 0;       ///< Aktualna klatka
    float frameTime = 0.1f;     ///< Czas wyœwietlania jednej klatki
    float timeAccumulator = 0.0f; ///< Akumulator czasu

public:
    /**
 * @brief Konstruktor sprite'a.
 * @param x Pozycja X.
 * @param y Pozycja Y.
 */
    SpriteObject(float x = 0, float y = 0) : BitmapObject() { pos.x = x; pos.y = y; }

    /**
* @brief Konfiguruje parametry sprite'a.
* @param fw Szerokoœæ klatki.
* @param fh Wysokoœæ klatki.
* @param frames Liczba klatek.
* @param timePerFrame Czas na klatkê.
*/

    void setupSprite(int fw, int fh, int frames, float timePerFrame = 0.1f) {
        frameWidth = fw;
        frameHeight = fh;
        frameCount = frames;
        frameTime = timePerFrame;
    }

 /**
* @brief Aktualizuje animacjê sprite'a.
* @param dt Delta time.
*/
    virtual void animate(float dt) override {
        if (bitmaps.empty()) return;

        timeAccumulator += dt;
        if (timeAccumulator >= frameTime) {
            currentFrame = (currentFrame + 1) % bitmaps.size(); // <- bez frameCount
            timeAccumulator = 0.0f;
        }
    }

    /**
 * @brief Rysuje aktualn¹ klatkê sprite'a.
 */
    virtual void draw() override {
        if (bitmaps.empty()) return;

        ALLEGRO_BITMAP* bmp = bitmaps[currentFrame]->get();
        if (!bmp) return;

        al_draw_bitmap(bmp, pos.x, pos.y, 0);
    }


};
