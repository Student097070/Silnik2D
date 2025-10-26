#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <ctime>
#include <Windows.h>

using namespace std;

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
    ALLEGRO_TIMER* timer = al_create_timer(1.0/144.0);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());

    al_start_timer(timer);

    time_t x=0;

    //Wymiary i polożenie początkowe przycisku do rozdzielczości
    float WorkspacePlace_w = 500, WorkspacePlace_h = 300;
    float ResolutionButton_w = 100, ResolutionButton_h = 20;  

    float ResolutionButton_x = 0;
    float ResolutionButton_y = 0;

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

        else if(ev.type == ALLEGRO_EVENT_TIMER){

            time(&x);

            string czasomierz = to_string(x);

            czasomierz.erase(0, 8);
          
            al_draw_text(Rozdzielczosc, al_map_rgb(255, 255, 255), 100, 100, ALLEGRO_ALIGN_CENTRE, czasomierz.c_str());
            
            al_flip_display();
        }
        // Zmiana rozmiaru okna
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
            al_acknowledge_resize(display);

            int new_w = al_get_display_width(display);
            int new_h = al_get_display_height(display);

            // ponowne wyśrodkowanie przycisku
            ResolutionButton_x = 0;
            ResolutionButton_y = 0;
            if (fullscreen == false) {
                WorkspacePlace_w = 400, WorkspacePlace_h = 300;
            }
            else
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

                };
                const int num_res = sizeof(resolutions) / sizeof(resolutions[0]);

                int next_index = 0;
                for (int i = 0; i < num_res; i++) {
                    if (current_w == resolutions[i].w && current_h == resolutions[i].h) {
                        next_index = (i + 1) % num_res; // przejście do kolejnej rozdzielczości w cyklu
                        break;
                    }
                }

                al_resize_display(display, resolutions[next_index].w, resolutions[next_index].h);

                // Wycentrowanie okna po zmianie rozdzielczości
                ALLEGRO_MONITOR_INFO info;
                al_get_monitor_info(0, &info);
                int screen_w = info.x2 - info.x1;
                int screen_h = info.y2 - info.y1;

                int win_x = (screen_w - resolutions[next_index].w) / 2;
                int win_y = (screen_h - resolutions[next_index].h) / 2;
                al_set_window_position(display, win_x, win_y);

                WorkspacePlace_w = resolutions[next_index].workspace_w;
                WorkspacePlace_h = resolutions[next_index].workspace_h;

                current_w = resolutions[next_index].w;
                current_h = resolutions[next_index].h;

                // ponowne wyśrodkowanie przycisku
                ResolutionButton_x = 0;
                ResolutionButton_y = 0;

                WorkspacePlace_x = (current_w / 2.0f) - (WorkspacePlace_w / 2.0f);
                WorkspacePlace_y = (current_h / 2.0f) - (WorkspacePlace_h / 2.0f);
            }
        }
        // Naciśnięcie ESC — zamknięcie
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            running = false;

        // Rysowanie
        al_clear_to_color(al_map_rgb(30, 30, 30));
        ALLEGRO_COLOR color = hovered ? al_map_rgb(0, 120, 255) : al_map_rgb(0, 0, 0);
        al_draw_filled_rectangle(ResolutionButton_x, ResolutionButton_y, ResolutionButton_x + ResolutionButton_w, ResolutionButton_y + ResolutionButton_h, color);
        al_draw_filled_rectangle(WorkspacePlace_x, WorkspacePlace_y, WorkspacePlace_x + WorkspacePlace_w, WorkspacePlace_y + WorkspacePlace_h, al_map_rgb(190, 190, 190));
        al_draw_rectangle(ResolutionButton_x, ResolutionButton_y, ResolutionButton_x + ResolutionButton_w, ResolutionButton_y + ResolutionButton_h, al_map_rgb(255, 255, 255), 2);
        al_draw_text(Rozdzielczosc, al_map_rgb(255, 255, 255), ResolutionButton_x + (ResolutionButton_w / 2.0), ResolutionButton_y + (ResolutionButton_h / 5), ALLEGRO_ALIGN_CENTRE, "ROZDZIELCZOSC");
        al_flip_display();
    }

    al_destroy_timer(timer);
    al_destroy_font(Rozdzielczosc);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}