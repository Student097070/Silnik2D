/**
 * @file Player.h
 * @brief Nag³ówek klasy Player – sterowalnej postaci w grze.
 * @details
 * - Dziedziczy po SpriteObject i UpdatableObject.
 * - Obs³uguje animacjê w zale¿noœci od kierunku ruchu.
 * - Wczytuje zestawy sprite’ów dla 8 kierunków ruchu.
 */

#pragma once
#include "SpriteObject.h"
#include "UpdatableObject.h"
#include "Include.h"

 /**
  * @enum Direction
  * @brief Kierunki ruchu postaci.
  */

enum class Direction {
    UP, UP_RIGHT, RIGHT, DOWN_RIGHT,
    DOWN, DOWN_LEFT, LEFT, UP_LEFT, NONE
};

/**
 * @class Player
 * @brief Klasa reprezentuj¹ca gracza.
 */

class Player : public SpriteObject, public UpdatableObject {
public:
    float speed; ///< Prêdkoœæ poruszania siê
    Direction dir = Direction::DOWN; // domyœlny kierunek
    std::map<Direction, std::vector<std::unique_ptr<BitmapHandler>>> sprites;  ///< Mapowanie kierunków na sprite'y

    Player(float x = 400, float y = 300, float speed = 200.0f)
        : SpriteObject(), UpdatableObject(), speed(speed)
    {
        pos.x = x;
        pos.y = y;

        // Wczytanie sprite'ów dla ka¿dego kierunku
        loadDirectionSprites();
        setDirectionSprites(Direction::DOWN);
    }

    void loadDirectionSprites() {
        for (int i = 0; i < 8; ++i) {
            ostringstream filename;
            filename << "sprity/Gangster/lewo/"
                << "Gangster_"
                << setw(2) << setfill('0') << i
                << "_3.png";                                 

            auto bmp = new BitmapHandler();
            if (!bmp->loadFromFile(filename.str())) {
                cerr << "Nie uda³o siê wczytaæ " << filename.str() << std::endl;
                delete bmp;
                continue;
            }
            sprites[Direction::LEFT].push_back(std::unique_ptr<BitmapHandler>(bmp));
        }
        for (int i = 0; i < 8; ++i) {
            ostringstream filename;
            filename << "sprity/Gangster/prawo/"
                << "Gangster_"
                << setw(2) << setfill('0') << i
                << "_7.png";

            auto bmp = new BitmapHandler();
            if (!bmp->loadFromFile(filename.str())) {
                cerr << "Nie uda³o siê wczytaæ " << filename.str() << std::endl;
                delete bmp;
                continue;
            }
            sprites[Direction::RIGHT].push_back(std::unique_ptr<BitmapHandler>(bmp));
        }
        for (int i = 0; i < 8; ++i) {
            ostringstream filename;
            filename << "sprity/Gangster/przod/"
                << "Gangster_"
                << setw(2) << setfill('0') << i
                << "_1.png";

            auto bmp = new BitmapHandler();
            if (!bmp->loadFromFile(filename.str())) {
                cerr << "Nie uda³o siê wczytaæ " << filename.str() << std::endl;
                delete bmp;
                continue;
            }
            sprites[Direction::DOWN].push_back(std::unique_ptr<BitmapHandler>(bmp));
        }
        for (int i = 0; i < 8; ++i) {
            ostringstream filename;
            filename << "sprity/Gangster/przod_lewo45stopni/"
                << "Gangster_"
                << setw(2) << setfill('0') << i
                << "_2.png";

            auto bmp = new BitmapHandler();
            if (!bmp->loadFromFile(filename.str())) {
                cerr << "Nie uda³o siê wczytaæ " << filename.str() << std::endl;
                delete bmp;
                continue;
            }
            sprites[Direction::DOWN_LEFT].push_back(std::unique_ptr<BitmapHandler>(bmp));
        }
        for (int i = 0; i < 8; ++i) {
            ostringstream filename;
            filename << "sprity/Gangster/przod_prawo45stopni/"
                << "Gangster_"
                << setw(2) << setfill('0') << i
                << "_8.png";

            auto bmp = new BitmapHandler();
            if (!bmp->loadFromFile(filename.str())) {
                cerr << "Nie uda³o siê wczytaæ " << filename.str() << std::endl;
                delete bmp;
                continue;
            }
            sprites[Direction::DOWN_RIGHT].push_back(std::unique_ptr<BitmapHandler>(bmp));
        }
        for (int i = 0; i < 8; ++i) {
            ostringstream filename;
            filename << "sprity/Gangster/tyl/"
                << "Gangster_"
                << setw(2) << setfill('0') << i
                << "_5.png";

            auto bmp = new BitmapHandler();
            if (!bmp->loadFromFile(filename.str())) {
                cerr << "Nie uda³o siê wczytaæ " << filename.str() << std::endl;
                delete bmp;
                continue;
            }
            sprites[Direction::UP].push_back(std::unique_ptr<BitmapHandler>(bmp));
        }
        for (int i = 0; i < 8; ++i) {
            ostringstream filename;
            filename << "sprity/Gangster/tyl_lewo45stopni/"
                << "Gangster_"
                << setw(2) << setfill('0') << i
                << "_4.png";

            auto bmp = new BitmapHandler();
            if (!bmp->loadFromFile(filename.str())) {
                cerr << "Nie uda³o siê wczytaæ " << filename.str() << std::endl;
                delete bmp;
                continue;
            }
            sprites[Direction::UP_LEFT].push_back(std::unique_ptr<BitmapHandler>(bmp));
        }
        for (int i = 0; i < 8; ++i) {
            ostringstream filename;
            filename << "sprity/Gangster/tyl_prawo45stopni/"
                << "Gangster_"
                << setw(2) << setfill('0') << i
                << "_6.png";

            auto bmp = new BitmapHandler();
            if (!bmp->loadFromFile(filename.str())) {
                cerr << "Nie uda³o siê wczytaæ " << filename.str() << std::endl;
                delete bmp;
                continue;
            }
            sprites[Direction::UP_RIGHT].push_back(std::unique_ptr<BitmapHandler>(bmp));
        }
    }

    void setDirectionSprites(Direction newDir) {
        dir = newDir;
        bitmaps.clear();
        for (auto& bmp : sprites[dir])
            bitmaps.push_back(bmp.get()); // same wskaŸniki, nie unique_ptr
        currentFrame = 0;
        timeAccumulator = 0.0f;
    }

    void update(float dt) override {
        ALLEGRO_KEYBOARD_STATE ks;
        al_get_keyboard_state(&ks);

        float dx = 0, dy = 0;

        if (al_key_down(&ks, ALLEGRO_KEY_UP)) dy -= 1;
        if (al_key_down(&ks, ALLEGRO_KEY_DOWN)) dy += 1;
        if (al_key_down(&ks, ALLEGRO_KEY_LEFT)) dx -= 1;
        if (al_key_down(&ks, ALLEGRO_KEY_RIGHT)) dx += 1;

        // Ustal kierunek na podstawie dx/dy
        Direction newDir = Direction::NONE;
        if (dx == 0 && dy < 0) newDir = Direction::UP;
        else if (dx > 0 && dy < 0) newDir = Direction::UP_RIGHT;
        else if (dx > 0 && dy == 0) newDir = Direction::RIGHT;
        else if (dx > 0 && dy > 0) newDir = Direction::DOWN_RIGHT;
        else if (dx == 0 && dy > 0) newDir = Direction::DOWN;
        else if (dx < 0 && dy > 0) newDir = Direction::DOWN_LEFT;
        else if (dx < 0 && dy == 0) newDir = Direction::LEFT;
        else if (dx < 0 && dy < 0) newDir = Direction::UP_LEFT;

        if (newDir != Direction::NONE && newDir != dir)
            setDirectionSprites(newDir);

        if (dx != 0 || dy != 0) {
            float len = sqrt(dx * dx + dy * dy);
            dx /= len; dy /= len;
            translate(dx * speed * dt, dy * speed * dt);
            animate(dt);
        }
    }
};
