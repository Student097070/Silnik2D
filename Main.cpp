#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

int main() {
    // Inicjalizacja Allegro
    if (!al_init()) {
        std::cerr << "B��d: nie uda�o si� zainicjalizowa� Allegro!" << std::endl;
        return -1;
    }

    // Inicjalizacja dodatku do rysowania kszta�t�w (primitives)
    if (!al_init_primitives_addon()) {
        std::cerr << "B��d: nie uda�o si� zainicjalizowa� allegro_primitives!" << std::endl;
        return -1;
    }

    // Utworzenie okna (ekranu)
    ALLEGRO_DISPLAY* display = al_create_display(640, 480);
    if (!display) {
        std::cerr << "B��d: nie uda�o si� utworzy� okna!" << std::endl;
        return -1;
    }

    // Ustawienie koloru t�a (bia�y)
    al_clear_to_color(al_map_rgb(255, 255, 255));

    // Rysowanie niebieskiego ko�a (x=320, y=240, promie�=100)
    al_draw_filled_circle(320, 240, 100, al_map_rgb(0, 0, 255));

    // Od�wie�enie ekranu
    al_flip_display();

    // Czekaj 5 sekund, �eby zobaczy� wynik
    al_rest(5.0);

    // 
    al_destroy_display(display);
    al_shutdown_primitives_addon();

    return 0;
}
