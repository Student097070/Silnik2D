#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

int main() {
    if (!al_init()) {
        std::cerr << "B³¹d inicjalizacji Allegro!" << std::endl;
        return -1;
    }

    al_init_primitives_addon();
    al_install_mouse();
    al_install_keyboard();

    const int WIDTH = 800, HEIGHT = 600;
    al_set_new_display_flags(ALLEGRO_RESIZABLE); // okno mo¿na zmieniaæ
    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());

    float btn_w = 200, btn_h = 80;
    float btn_x = (WIDTH / 2.0f) - (btn_w / 2.0f);
    float btn_y = (HEIGHT / 2.0f) - (btn_h / 2.0f);

    bool running = true;
    bool hovered = false;

    int current_w = 800;
    int current_h = 640;

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        // Zamkniêcie okna
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;

        // Zmiana rozmiaru okna
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
            al_acknowledge_resize(display);

            int new_w = al_get_display_width(display);
            int new_h = al_get_display_height(display);

            // ponowne wyœrodkowanie przycisku
            btn_x = (new_w / 2.0f) - (btn_w / 2.0f);
            btn_y = (new_h / 2.0f) - (btn_h / 2.0f);
        }

        // Ruch myszy – efekt hover
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (ev.mouse.x >= btn_x && ev.mouse.x <= btn_x + btn_w &&
                ev.mouse.y >= btn_y && ev.mouse.y <= btn_y + btn_h) {
                hovered = true;
            }
            else {
                hovered = false;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (ev.mouse.x >= btn_x && ev.mouse.x <= btn_x + btn_w &&
                ev.mouse.y >= btn_y && ev.mouse.y <= btn_y + btn_h) {
               
                if (current_w == 800 && current_h == 640) {
                    al_resize_display(display, 1800, 600);

                    current_w = 1800;
                    current_h = 600;
               }
                else if (current_w == 1800 && current_h == 600) {
                    al_resize_display(display, 800, 640);
                    current_w = 800;
                    current_h = 640;
                }
                int new_w = al_get_display_width(display);
                int new_h = al_get_display_height(display);

                // ponowne wyœrodkowanie przycisku
                btn_x = (new_w / 2.0f) - (btn_w / 2.0f);
                btn_y = (new_h / 2.0f) - (btn_h / 2.0f);
            }
        }
        // Naciœniêcie ESC — zamkniêcie
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            running = false;

        // Rysowanie
        al_clear_to_color(al_map_rgb(30, 30, 30));
        ALLEGRO_COLOR color = hovered ? al_map_rgb(0, 120, 255) : al_map_rgb(0, 0, 0);
        al_draw_filled_rectangle(btn_x, btn_y, btn_x + btn_w, btn_y + btn_h, color);
        al_draw_rectangle(btn_x, btn_y, btn_x + btn_w, btn_y + btn_h, al_map_rgb(255, 255, 255), 2);
        al_flip_display();
    }

    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}

