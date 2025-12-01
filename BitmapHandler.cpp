#include "BitmapHandler.h"

BitmapHandler::~BitmapHandler() {
    destroy();
}

bool BitmapHandler::create(int w, int h) {
    destroy();
    bitmap = al_create_bitmap(w, h);
    return bitmap != nullptr;
}

bool BitmapHandler::loadFromFile(const string& filename) {
    destroy();
    bitmap = al_load_bitmap(filename.c_str());
    return bitmap != nullptr;
}

bool BitmapHandler::saveToFile(const string& filename) {
    return bitmap ? al_save_bitmap(filename.c_str(), bitmap) : false;
}

void BitmapHandler::destroy() {
    if (bitmap) {
        al_destroy_bitmap(bitmap);
        bitmap = nullptr;
    }
}

void BitmapHandler::drawTo(BitmapHandler& target, float x, float y) {
    if (!bitmap || !target.get()) return;

    ALLEGRO_BITMAP* prev = al_get_target_bitmap();
    al_set_target_bitmap(target.get());
    al_draw_bitmap(bitmap, x, y, 0);
    al_set_target_bitmap(prev);
}

void BitmapHandler::drawRegionTo(BitmapHandler& target, float sx, float sy, float sw,
    float sh, float dx, float dy) {
    if (!bitmap || !target.get()) return;

    ALLEGRO_BITMAP* prev = al_get_target_bitmap();
    al_set_target_bitmap(target.get());

    al_draw_bitmap_region(bitmap, sx, sy, sw, sh, dx, dy, 0);

    al_set_target_bitmap(prev);
}
