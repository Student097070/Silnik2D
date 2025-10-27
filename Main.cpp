#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <ctime>
#include <Windows.h>

using namespace std;

struct Resolution {
    int w, h;
    int workspace_w, workspace_h;
};

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

    // cykl dostępnych rozdzielczości
    Resolution resolutions[] = {
        {800, 640, 400, 300},
        {1024, 768, 500, 350},
        {1152, 864, 600, 400},
        {1280, 720, 700, 450},
        {1920,1080, 1300, 700}
    };
    const int num_res = sizeof(resolutions) / sizeof(resolutions[0]);
    int current_index = 0;

    const int WIDTH = resolutions[current_index].w, HEIGHT = resolutions[current_index].h;
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_FONT* Rozdzielczosc = al_load_ttf_font("Arial.ttf", 11, 0);
    ALLEGRO_FONT* TimerFont = al_load_ttf_font("Arial.ttf", 24, 0); // Większa czcionka dla timera
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 144.0);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());

    al_start_timer(timer);

    time_t x = 0;

    // Wymiary i polożenie początkowe
    float WorkspacePlace_w = resolutions[current_index].workspace_w;
    float WorkspacePlace_h = resolutions[current_index].workspace_h;
    float ResolutionButton_w = 100, ResolutionButton_h = 20;

    float ResolutionButton_x = 0;
    float ResolutionButton_y = 0;

    float WorkspacePlace_x = (WIDTH / 2.0f) - (WorkspacePlace_w / 2.0f);
    float WorkspacePlace_y = (HEIGHT / 2.0f) - (WorkspacePlace_h / 2.0f);

    // Pozycja timera - nad polem roboczym
    float Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
    float Timer_y = WorkspacePlace_y - 40; // 40 pikseli nad polem roboczym

    bool running = true;
    bool hovered = false;
    bool fullscreen = false;

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        // Zamknięcie okna
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;

        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            time(&x);
            string czasomierz = to_string(x);
            czasomierz.erase(0, 8);

            // Rysowanie
            al_clear_to_color(al_map_rgb(30, 30, 30));
            ALLEGRO_COLOR color = hovered ? al_map_rgb(0, 120, 255) : al_map_rgb(0, 0, 0);

            al_draw_filled_rectangle(ResolutionButton_x, ResolutionButton_y,
                ResolutionButton_x + ResolutionButton_w,
                ResolutionButton_y + ResolutionButton_h, color);
            al_draw_filled_rectangle(WorkspacePlace_x, WorkspacePlace_y,
                WorkspacePlace_x + WorkspacePlace_w,
                WorkspacePlace_y + WorkspacePlace_h,
                al_map_rgb(190, 190, 190));
            al_draw_rectangle(ResolutionButton_x, ResolutionButton_y,
                ResolutionButton_x + ResolutionButton_w,
                ResolutionButton_y + ResolutionButton_h,
                al_map_rgb(255, 255, 255), 2);
            al_draw_text(Rozdzielczosc, al_map_rgb(255, 255, 255),
                ResolutionButton_x + (ResolutionButton_w / 2.0),
                ResolutionButton_y + (ResolutionButton_h / 5),
                ALLEGRO_ALIGN_CENTRE, "ROZDZIELCZOSC");

            // Rysowanie timera - większa czcionka, widoczny kolor, wyśrodkowany nad polem roboczym
            al_draw_text(TimerFont, al_map_rgb(255, 215, 0), // Złoty kolor dla lepszej widoczności
                Timer_x, Timer_y, ALLEGRO_ALIGN_CENTRE, czasomierz.c_str());

            al_flip_display();
        }
        // Zmiana rozmiaru okna
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
            al_acknowledge_resize(display);

            int new_w = al_get_display_width(display);
            int new_h = al_get_display_height(display);

            ResolutionButton_x = 0;
            ResolutionButton_y = 0;

            if (!fullscreen) {
                WorkspacePlace_w = resolutions[current_index].workspace_w;
                WorkspacePlace_h = resolutions[current_index].workspace_h;
            }
            else {
                WorkspacePlace_w = 800;
                WorkspacePlace_h = 500;
            }

            WorkspacePlace_x = (new_w / 2.0f) - (WorkspacePlace_w / 2.0f);
            WorkspacePlace_y = (new_h / 2.0f) - (WorkspacePlace_h / 2.0f);

            // Aktualizacja pozycji timera
            Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
            Timer_y = WorkspacePlace_y - 40;
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

                current_index = (current_index + 1) % num_res;
                al_resize_display(display, resolutions[current_index].w, resolutions[current_index].h);

                WorkspacePlace_w = resolutions[current_index].workspace_w;
                WorkspacePlace_h = resolutions[current_index].workspace_h;
                WorkspacePlace_x = (resolutions[current_index].w / 2.0f) - (WorkspacePlace_w / 2.0f);
                WorkspacePlace_y = (resolutions[current_index].h / 2.0f) - (WorkspacePlace_h / 2.0f);

                // Aktualizacja pozycji timera
                Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
                Timer_y = WorkspacePlace_y - 40;

                // Centrowanie okna
                ALLEGRO_MONITOR_INFO info;
                al_get_monitor_info(0, &info);
                int screen_w = info.x2 - info.x1;
                int screen_h = info.y2 - info.y1;
                int win_x = (screen_w - resolutions[current_index].w) / 2;
                int win_y = (screen_h - resolutions[current_index].h) / 2;
                al_set_window_position(display, win_x, win_y);
            }
        }
        // Obsługa klawiszy
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                running = false;
                break;

            case ALLEGRO_KEY_F: // F – przełącza pełny ekran
                fullscreen = !fullscreen;
                al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen);
                if (fullscreen) {
                    WorkspacePlace_w = 800;
                    WorkspacePlace_h = 500;
                    WorkspacePlace_x = (1920 / 2.0f) - (800 / 2.0f);
                    WorkspacePlace_y = (1080 / 2.0f) - (500 / 2.0f);

                    // Aktualizacja pozycji timera w trybie pełnoekranowym
                    Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
                    Timer_y = WorkspacePlace_y - 40;
                }
                else {
                    // PRZYWRÓĆ ORYGINALNĄ ROZDZIELCZOŚĆ
                    al_resize_display(display, resolutions[current_index].w, resolutions[current_index].h);

                    // PRZYWRÓĆ ORYGINALNE WYMIARY WORKSPACE
                    WorkspacePlace_w = resolutions[current_index].workspace_w;
                    WorkspacePlace_h = resolutions[current_index].workspace_h;
                    WorkspacePlace_x = (resolutions[current_index].w / 2.0f) - (WorkspacePlace_w / 2.0f);
                    WorkspacePlace_y = (resolutions[current_index].h / 2.0f) - (WorkspacePlace_h / 2.0f);

                    // Aktualizacja pozycji timera
                    Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
                    Timer_y = WorkspacePlace_y - 40;

                    // CENTROWANIE OKNA
                    ALLEGRO_MONITOR_INFO info;
                    al_get_monitor_info(0, &info);
                    int screen_w = info.x2 - info.x1;
                    int screen_h = info.y2 - info.y1;
                    int win_x = (screen_w - resolutions[current_index].w) / 2;
                    int win_y = (screen_h - resolutions[current_index].h) / 2;
                    al_set_window_position(display, win_x, win_y);
                }
                break;
            }
        }
    }

    al_destroy_timer(timer);
    al_destroy_font(Rozdzielczosc);
    al_destroy_font(TimerFont); // Pamiętaj o zniszczeniu nowej czcionki
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}