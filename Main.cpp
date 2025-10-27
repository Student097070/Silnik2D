#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <Windows.h> 

using namespace std;

struct Resolution {
    int w, h;
    int workspace_w, workspace_h;
};

void logError(const string& errorMessage) {
    ofstream errorFile("error_log.txt", ios::app);
    if (errorFile.is_open()) {
        time_t now = time(nullptr);
        char timeStr[100];
        ctime_s(timeStr, sizeof(timeStr), &now);
        timeStr[strlen(timeStr) - 1] = '\0';
        errorFile << "[" << timeStr << "] " << errorMessage << endl;
        errorFile.close();
    }
    cerr << errorMessage << endl;
}

int main() {
    // Inicjalizacja pliku logów
    ofstream errorFile("error_log.txt");
    if (errorFile.is_open()) {
        errorFile << "=== Log błędów programu ===" << endl;
        errorFile.close();
    }

    // Inicjalizacja Allegro z obsługą błędów
    if (!al_init()) { logError("Błąd inicjalizacji Allegro!"); return -1; }
    if (!al_init_font_addon()) { logError("Błąd inicjalizacji dodatku czcionek!"); return -1; }
    if (!al_init_ttf_addon()) { logError("Błąd inicjalizacji dodatku TTF!"); return -1; }
    if (!al_init_primitives_addon()) { logError("Błąd inicjalizacji primitives!"); return -1; }
    if (!al_install_mouse()) { logError("Błąd inicjalizacji myszy!"); return -1; }
    if (!al_install_keyboard()) { logError("Błąd inicjalizacji klawiatury!"); return -1; }

    // Dostępne rozdzielczości
    Resolution resolutions[] = {
        {800, 640, 400, 300},
        {1024, 768, 500, 350},
        {1152, 864, 600, 400},
        {1280, 720, 1000, 500},
        {1920, 1080, 1200, 750}
    };
    const int num_res = sizeof(resolutions) / sizeof(resolutions[0]);
    int current_index = 0;

    // Utworzenie display
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY* display = al_create_display(resolutions[current_index].w, resolutions[current_index].h);
    if (!display) { logError("Błąd tworzenia display!"); return -1; }
    al_set_window_title(display, "Timer Allegro");

    // Utworzenie kolejki zdarzeń
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue) { logError("Błąd tworzenia kolejki zdarzeń!"); al_destroy_display(display); return -1; }

    // Ładowanie czcionek
    ALLEGRO_FONT* Rozdzielczosc = al_load_ttf_font("Arial.ttf", 11, 0);
    if (!Rozdzielczosc) {
        logError("Błąd ładowania czcionki Arial.ttf (11px)!");
        Rozdzielczosc = al_create_builtin_font();
        if (!Rozdzielczosc) { logError("Nie udało się załadować żadnej czcionki!"); return -1; }
    }

    ALLEGRO_FONT* TimerFont = al_load_ttf_font("Arial.ttf", 24, 0);
    if (!TimerFont) {
        logError("Błąd ładowania czcionki Arial.ttf (24px)!");
        TimerFont = Rozdzielczosc;
    }

    // Utworzenie timera
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 144.0);
    if (!timer) { logError("Błąd tworzenia timera!"); return -1; }

    // Rejestracja źródeł zdarzeń
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());

    al_start_timer(timer);

    // Timer
    time_t start_time = time(nullptr);
    auto resetTimer = [&]() { start_time = time(nullptr); };

    // Wymiary i położenie przycisków
    float ResolutionButton_w = 100, ResolutionButton_h = 20;
    float ResetButton_w = 100, ResetButton_h = 20;
    float ResolutionButton_x = 0, ResolutionButton_y = 0;
    float ResetButton_x = 110, ResetButton_y = 0;

    // Workspace
    float WorkspacePlace_w = resolutions[current_index].workspace_w;
    float WorkspacePlace_h = resolutions[current_index].workspace_h;
    float WorkspacePlace_x = (resolutions[current_index].w / 2.0f) - (WorkspacePlace_w / 2.0f);
    float WorkspacePlace_y = (resolutions[current_index].h / 2.0f) - (WorkspacePlace_h / 2.0f);

    // Pozycja timera
    float Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
    float Timer_y = WorkspacePlace_y - 40;

    bool running = true, resolution_hovered = false, reset_hovered = false, fullscreen = false;

    // Lambda do centrowania okna
    auto centerWindow = [&](ALLEGRO_DISPLAY* disp, int width, int height) {
        ALLEGRO_MONITOR_INFO info;
        if (al_get_monitor_info(0, &info)) {
            int screen_w = info.x2 - info.x1;
            int screen_h = info.y2 - info.y1;
            int win_x = (screen_w - width) / 2;
            int win_y = (screen_h - height) / 2;
            al_set_window_position(disp, win_x, win_y);
        }
        else {
            logError("Błąd pobierania informacji o monitorze!");
        }
        };

    centerWindow(display, resolutions[current_index].w, resolutions[current_index].h);
    logError("Program uruchomiony pomyślnie");

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            time_t elapsed = time(nullptr) - start_time;
            string czasomierz = to_string(elapsed);

            // Rysowanie
            al_clear_to_color(al_map_rgb(30, 30, 30));

            ALLEGRO_COLOR resolution_color = resolution_hovered ? al_map_rgb(0, 120, 255) : al_map_rgb(0, 0, 0);
            ALLEGRO_COLOR reset_color = reset_hovered ? al_map_rgb(255, 100, 100) : al_map_rgb(0, 0, 0);

            // Przycisk rozdzielczości
            al_draw_filled_rectangle(ResolutionButton_x, ResolutionButton_y,
                ResolutionButton_x + ResolutionButton_w, ResolutionButton_y + ResolutionButton_h, resolution_color);
            al_draw_rectangle(ResolutionButton_x, ResolutionButton_y,
                ResolutionButton_x + ResolutionButton_w, ResolutionButton_y + ResolutionButton_h,
                al_map_rgb(255, 255, 255), 2);
            al_draw_text(Rozdzielczosc, al_map_rgb(255, 255, 255),
                ResolutionButton_x + (ResolutionButton_w / 2.0), ResolutionButton_y + 4,
                ALLEGRO_ALIGN_CENTRE, "ROZDZIELCZOSC");

            // Przycisk reset
            al_draw_filled_rectangle(ResetButton_x, ResetButton_y,
                ResetButton_x + ResetButton_w, ResetButton_y + ResetButton_h, reset_color);
            al_draw_rectangle(ResetButton_x, ResetButton_y,
                ResetButton_x + ResetButton_w, ResetButton_y + ResetButton_h,
                al_map_rgb(255, 255, 255), 2);
            al_draw_text(Rozdzielczosc, al_map_rgb(255, 255, 255),
                ResetButton_x + (ResetButton_w / 2.0), ResetButton_y + 4,
                ALLEGRO_ALIGN_CENTRE, "RESET TIMERA");

            // Workspace
            al_draw_filled_rectangle(WorkspacePlace_x, WorkspacePlace_y,
                WorkspacePlace_x + WorkspacePlace_w, WorkspacePlace_y + WorkspacePlace_h,
                al_map_rgb(190, 190, 190));

            // Timer
            al_draw_text(TimerFont, al_map_rgb(255, 215, 0),
                Timer_x, Timer_y, ALLEGRO_ALIGN_CENTRE, czasomierz.c_str());

            al_flip_display();
        }
        // OBSŁUGA RĘCZNEJ ZMIANY ROZMIARU OKNA
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
            al_acknowledge_resize(display);
            int new_w = al_get_display_width(display);
            int new_h = al_get_display_height(display);

            // Przelicz pozycje dla nowego rozmiaru
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
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            resolution_hovered =
                (ev.mouse.x >= ResolutionButton_x && ev.mouse.x <= ResolutionButton_x + ResolutionButton_w &&
                    ev.mouse.y >= ResolutionButton_y && ev.mouse.y <= ResolutionButton_y + ResolutionButton_h);

            reset_hovered =
                (ev.mouse.x >= ResetButton_x && ev.mouse.x <= ResetButton_x + ResetButton_w &&
                    ev.mouse.y >= ResetButton_y && ev.mouse.y <= ResetButton_y + ResetButton_h);
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (resolution_hovered) {
                current_index = (current_index + 1) % num_res;
                if (!al_resize_display(display, resolutions[current_index].w, resolutions[current_index].h))
                    logError("Błąd zmiany rozdzielczości!");
                al_acknowledge_resize(display);

                WorkspacePlace_w = resolutions[current_index].workspace_w;
                WorkspacePlace_h = resolutions[current_index].workspace_h;
                WorkspacePlace_x = (resolutions[current_index].w / 2.0f) - (WorkspacePlace_w / 2.0f);
                WorkspacePlace_y = (resolutions[current_index].h / 2.0f) - (WorkspacePlace_h / 2.0f);

                // Aktualizacja pozycji timera
                Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
                Timer_y = WorkspacePlace_y - 40;

                centerWindow(display, resolutions[current_index].w, resolutions[current_index].h);
            }
            else if (reset_hovered) {
                resetTimer();
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                running = false;
                break;
            case ALLEGRO_KEY_F:
                fullscreen = !fullscreen;
                if (!al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen))
                    logError("Błąd przełączania trybu pełnoekranowego!");

                if (fullscreen) {
                    WorkspacePlace_w = 800;
                    WorkspacePlace_h = 500;
                    WorkspacePlace_x = (1920 / 2.0f) - (800 / 2.0f);
                    WorkspacePlace_y = (1080 / 2.0f) - (500 / 2.0f);
                }
                else {
                    al_resize_display(display, resolutions[current_index].w, resolutions[current_index].h);
                    WorkspacePlace_w = resolutions[current_index].workspace_w;
                    WorkspacePlace_h = resolutions[current_index].workspace_h;
                    WorkspacePlace_x = (resolutions[current_index].w / 2.0f) - (WorkspacePlace_w / 2.0f);
                    WorkspacePlace_y = (resolutions[current_index].h / 2.0f) - (WorkspacePlace_h / 2.0f);
                    centerWindow(display, resolutions[current_index].w, resolutions[current_index].h);
                }

                // Aktualizacja pozycji timera
                Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
                Timer_y = WorkspacePlace_y - 40;
                break;
            case ALLEGRO_KEY_R:
                resetTimer();
                break;
            }
        }
    }

    logError("Program zakończony pomyślnie");

    // Czyszczenie zasobów
    al_destroy_timer(timer);
    al_destroy_font(Rozdzielczosc);
    if (TimerFont != Rozdzielczosc) al_destroy_font(TimerFont);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}