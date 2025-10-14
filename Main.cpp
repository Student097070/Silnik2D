#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

int main() {
    // Inicjalizacja Allegro
    if (!al_init()) {
        std::cerr << "B³¹d: nie uda³o siê zainicjalizowaæ Allegro!" << std::endl;
        return -1;
    }

    // Inicjalizacja dodatku do rysowania kszta³tów (primitives)
    if (!al_init_primitives_addon()) {
        std::cerr << "B³¹d: nie uda³o siê zainicjalizowaæ allegro_primitives!" << std::endl;
        return -1;
    }

    // Utworzenie okna (ekranu)
    ALLEGRO_DISPLAY* display = al_create_display(640, 480);
    if (!display) {
        std::cerr << "B³¹d: nie uda³o siê utworzyæ okna!" << std::endl;
        return -1;
    }

    // Ustawienie koloru t³a (bia³y)
    al_clear_to_color(al_map_rgb(255, 255, 255));

    // Rysowanie niebieskiego ko³a (x=320, y=240, promieñ=100)
    al_draw_filled_circle(320, 240, 100, al_map_rgb(0, 0, 255));

    // Odœwie¿enie ekranu
    al_flip_display();

    // Czekaj 5 sekund, ¿eby zobaczyæ wynik
    al_rest(5.0);

    // 
    al_destroy_display(display);
    al_shutdown_primitives_addon();

    return 0;
}
