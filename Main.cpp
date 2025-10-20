#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int main() {
    if (!al_init()) {
        std::cerr << "Błąd inicjalizacji Allegro!" << std::endl;
        return -1;
    }
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_mouse();
    al_install_keyboard();

    const int WIDTH = 800, HEIGHT = 600;
    al_set_new_display_flags(ALLEGRO_RESIZABLE); // okno można zmieniać
    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_FONT* Rozdzielczosc = al_load_ttf_font("Arial.ttf", 11, 0);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());


    //Wymiary i polożenie początkowe przycisku do rozdzielczości
    float WorkspacePlace_w = 500, WorkspacePlace_h = 300;
    float ResolutionButton_w = 100, ResolutionButton_h = 20;
    /*float ResolutionButton_x = (WIDTH / 2.0f) - (ResolutionButton_w / 2.0f);
    float ResolutionButton_y = (HEIGHT / 2.0f) - (ResolutionButton_h / 2.0f); położenie na środku*/

    float ResolutionButton_x = 0;// (ResolutionButton_w / 2.0f);
    float ResolutionButton_y = 0;// (ResolutionButton_h / 2.0f); //położenie w lewym górnym rogu

    float WorkspacePlace_x = (WIDTH / 2.0f) - (WorkspacePlace_w / 2.0f);
    float WorkspacePlace_y = (HEIGHT / 2.0f) - (WorkspacePlace_h / 2.0f);

    bool running = true;
    bool hovered = false;

    int current_w = 800;
    int current_h = 640;

    bool fullscreen = false;

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        // Zamknięcie okna
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;

        // Zmiana rozmiaru okna
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
            al_acknowledge_resize(display);

            int new_w = al_get_display_width(display);
            int new_h = al_get_display_height(display);

            // ponowne wyśrodkowanie przycisku
            ResolutionButton_x = 0;
            ResolutionButton_y = 0;

            WorkspacePlace_w = 1000, WorkspacePlace_h = 600;

            WorkspacePlace_x = (new_w / 2.0f) - (WorkspacePlace_w / 2.0f);
            WorkspacePlace_y = (new_h / 2.0f) - (WorkspacePlace_h / 2.0f);
        }

        // Ruch myszy – efekt hover
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (ev.mouse.x >= ResolutionButton_x && ev.mouse.x <= ResolutionButton_x + ResolutionButton_w &&
                ev.mouse.y >= ResolutionButton_y && ev.mouse.y <= ResolutionButton_y + ResolutionButton_h) {
                hovered = true;
            }
            else {
                hovered = false;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (ev.mouse.x >= ResolutionButton_x && ev.mouse.x <= ResolutionButton_x + ResolutionButton_w &&
                ev.mouse.y >= ResolutionButton_y && ev.mouse.y <= ResolutionButton_y + ResolutionButton_h) {

                if (current_w == 800 && current_h == 640) {
                    al_resize_display(display, 1024, 768);

                    WorkspacePlace_w = 600, WorkspacePlace_h = 400;

                    current_w = 1024;
                    current_h = 768;
                }
                else if (current_w == 1024 && current_h == 768) {
                    al_resize_display(display, 1152, 864);

                    WorkspacePlace_w = 600, WorkspacePlace_h = 400;

                    current_w = 1152;
                    current_h = 864;
                }
                else if (current_w == 1152 && current_h == 864) {
                    al_resize_display(display, 1280, 720);

                    WorkspacePlace_w = 600, WorkspacePlace_h = 400;

                    current_w = 1280;
                    current_h = 720;
                }
                else if (current_w == 1280 && current_h == 720) {
                    al_resize_display(display, 1280, 1024);

                    WorkspacePlace_w = 700, WorkspacePlace_h = 500;

                    current_w = 1280;
                    current_h = 1024;
                }
                else if (current_w == 1280 && current_h == 1024) {
                    al_resize_display(display, 800, 640);

                    WorkspacePlace_w = 500, WorkspacePlace_h = 300;

                    current_w = 800;
                    current_h = 640;
                }
                int new_w = al_get_display_width(display);
                int new_h = al_get_display_height(display);

                // ponowne wyśrodkowanie przycisku
                ResolutionButton_x = 0;
                ResolutionButton_y = 0;

                WorkspacePlace_x = (current_w / 2.0f) - (WorkspacePlace_w / 2.0f);
                WorkspacePlace_y = (current_h / 2.0f) - (WorkspacePlace_h / 2.0f);
            }
        }
        // Naciśnięcie ESC — zamknięcie
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                running = false;
                break;

            case ALLEGRO_KEY_F: // F – przełącza pełny ekran
                fullscreen = !fullscreen;
                al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen);
                break;
            }
        }

        // Rysowanie
        al_clear_to_color(al_map_rgb(30, 30, 30));
        ALLEGRO_COLOR color = hovered ? al_map_rgb(0, 120, 255) : al_map_rgb(0, 0, 0);
        al_draw_filled_rectangle(ResolutionButton_x, ResolutionButton_y, ResolutionButton_x + ResolutionButton_w, ResolutionButton_y + ResolutionButton_h, color);
        al_draw_filled_rectangle(WorkspacePlace_x, WorkspacePlace_y, WorkspacePlace_x + WorkspacePlace_w, WorkspacePlace_y + WorkspacePlace_h, al_map_rgb(190, 190, 190));
        al_draw_rectangle(ResolutionButton_x, ResolutionButton_y, ResolutionButton_x + ResolutionButton_w, ResolutionButton_y + ResolutionButton_h, al_map_rgb(255, 255, 255), 2);
        al_draw_text(Rozdzielczosc, al_map_rgb(255, 255, 255), ResolutionButton_x + (ResolutionButton_w / 2.0), ResolutionButton_y + (ResolutionButton_h / 5), ALLEGRO_ALIGN_CENTRE, "ROZDZIELCZOSC");
        al_flip_display();
    }

    al_destroy_font(Rozdzielczosc);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}