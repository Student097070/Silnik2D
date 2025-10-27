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

class Engine {
private:
    ALLEGRO_DISPLAY* display = nullptr;
    ALLEGRO_EVENT_QUEUE* queue = nullptr;
    ALLEGRO_TIMER* timer = nullptr;
    ALLEGRO_FONT* fontSmall = nullptr;
    ALLEGRO_FONT* fontLarge = nullptr;

    Resolution resolutions[5] = {
        {800, 640, 400, 300},
        {1024, 768, 500, 350},
        {1152, 864, 600, 400},
        {1280, 720, 1000, 500},
        {1920, 1080, 1200, 750}
    };
    int current_index = 0;

    bool running = true;
    bool fullscreen = false;
    bool resolution_hovered = false;
    bool reset_hovered = false;

    float ResolutionButton_x = 0, ResolutionButton_y = 0;
    float ResolutionButton_w = 100, ResolutionButton_h = 20;
    float ResetButton_x = 110, ResetButton_y = 0;
    float ResetButton_w = 100, ResetButton_h = 20;
    float WorkspacePlace_x, WorkspacePlace_y, WorkspacePlace_w, WorkspacePlace_h;
    float Timer_x, Timer_y;

    time_t start_time = time(nullptr);

public:
    Engine() {
        initLog();
        if (!initAllegro()) exit(-1);
        createDisplay();
        loadFonts();
        createTimer();
        registerEventSources();
        setupWorkspace();
        centerWindow();
        logError("Silnik uruchomiony pomyślnie");
    }

    ~Engine() {
        cleanup();
    }

    void run() {
        al_start_timer(timer);
        while (running) {
            ALLEGRO_EVENT ev;
            al_wait_for_event(queue, &ev);
            handleEvent(ev);
        }
        logError("Silnik zakończył działanie pomyślnie");
    }

private:
    void initLog() {
        ofstream errorFile("error_log.txt");
        if (errorFile.is_open()) {
            errorFile << "=== Log błędów programu ===" << endl;
            errorFile.close();
        }
    }

    static void logError(const string& errorMessage) {
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

    bool initAllegro() {
        if (!al_init()) { logError("Błąd inicjalizacji Allegro!"); return false; }
        if (!al_init_font_addon()) { logError("Błąd inicjalizacji fontów!"); return false; }
        if (!al_init_ttf_addon()) { logError("Błąd inicjalizacji TTF!"); return false; }
        if (!al_init_primitives_addon()) { logError("Błąd inicjalizacji primitives!"); return false; }
        if (!al_install_mouse()) { logError("Błąd inicjalizacji myszy!"); return false; }
        if (!al_install_keyboard()) { logError("Błąd inicjalizacji klawiatury!"); return false; }
        return true;
    }

    void createDisplay() {
        al_set_new_display_flags(ALLEGRO_RESIZABLE);
        display = al_create_display(resolutions[current_index].w, resolutions[current_index].h);
        if (!display) { logError("Błąd tworzenia display!"); exit(-1); }
        al_set_window_title(display, "Engine Allegro");
        queue = al_create_event_queue();
        if (!queue) { logError("Błąd tworzenia kolejki zdarzeń!"); exit(-1); }
    }

    void loadFonts() {
        fontSmall = al_load_ttf_font("Arial.ttf", 11, 0);
        if (!fontSmall) {
            logError("Błąd ładowania czcionki Arial.ttf (11px)!");
            fontSmall = al_create_builtin_font();
        }
        fontLarge = al_load_ttf_font("Arial.ttf", 24, 0);
        if (!fontLarge) fontLarge = fontSmall;
    }

    void createTimer() {
        timer = al_create_timer(1.0 / 144.0);
        if (!timer) { logError("Błąd tworzenia timera!"); exit(-1); }
    }

    void registerEventSources() {
        al_register_event_source(queue, al_get_display_event_source(display));
        al_register_event_source(queue, al_get_timer_event_source(timer));
        al_register_event_source(queue, al_get_mouse_event_source());
        al_register_event_source(queue, al_get_keyboard_event_source());
    }

    void setupWorkspace() {
        WorkspacePlace_w = resolutions[current_index].workspace_w;
        WorkspacePlace_h = resolutions[current_index].workspace_h;
        WorkspacePlace_x = (resolutions[current_index].w / 2.0f) - (WorkspacePlace_w / 2.0f);
        WorkspacePlace_y = (resolutions[current_index].h / 2.0f) - (WorkspacePlace_h / 2.0f);
        Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
        Timer_y = WorkspacePlace_y - 40;
    }

    void centerWindow() {
        ALLEGRO_MONITOR_INFO info;
        if (al_get_monitor_info(0, &info)) {
            int screen_w = info.x2 - info.x1;
            int screen_h = info.y2 - info.y1;
            int win_x = (screen_w - resolutions[current_index].w) / 2;
            int win_y = (screen_h - resolutions[current_index].h) / 2;
            al_set_window_position(display, win_x, win_y);
        }
    }

    void resetTimer() {
        start_time = time(nullptr);
    }

    void handleEvent(ALLEGRO_EVENT& ev) {
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;
        else if (ev.type == ALLEGRO_EVENT_TIMER)
            draw();
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
            handleMouseMove(ev.mouse.x, ev.mouse.y);
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            handleMouseClick();
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            handleKey(ev.keyboard.keycode);
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            handleResize();
    }

    void handleMouseMove(int x, int y) {
        resolution_hovered =
            (x >= ResolutionButton_x && x <= ResolutionButton_x + ResolutionButton_w &&
                y >= ResolutionButton_y && y <= ResolutionButton_y + ResolutionButton_h);
        reset_hovered =
            (x >= ResetButton_x && x <= ResetButton_x + ResetButton_w &&
                y >= ResetButton_y && y <= ResetButton_y + ResetButton_h);
    }

    void handleMouseClick() {
        if (resolution_hovered) changeResolution();
        else if (reset_hovered) resetTimer();
    }

    void handleKey(int key) {
        switch (key) {
        case ALLEGRO_KEY_ESCAPE: running = false; break;
        case ALLEGRO_KEY_R: resetTimer(); break;
        case ALLEGRO_KEY_F: toggleFullscreen(); break;
        }
    }

    void handleResize() {
        al_acknowledge_resize(display);
        int new_w = al_get_display_width(display);
        int new_h = al_get_display_height(display);
        WorkspacePlace_x = (new_w / 2.0f) - (WorkspacePlace_w / 2.0f);
        WorkspacePlace_y = (new_h / 2.0f) - (WorkspacePlace_h / 2.0f);
        Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
        Timer_y = WorkspacePlace_y - 40;
    }

    void changeResolution() {
        current_index = (current_index + 1) % 5;
        al_resize_display(display, resolutions[current_index].w, resolutions[current_index].h);
        setupWorkspace();
        centerWindow();
    }

    void toggleFullscreen() {
        fullscreen = !fullscreen;
        if (!al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen))
            logError("Błąd przełączania pełnego ekranu!");
    }

    void draw() {
        time_t elapsed = time(nullptr) - start_time;
        string timerText = to_string(elapsed);

        al_clear_to_color(al_map_rgb(30, 30, 30));

        // Przycisk rozdzielczości
        ALLEGRO_COLOR resolution_color = resolution_hovered ? al_map_rgb(0, 120, 255) : al_map_rgb(0, 0, 0);
        al_draw_filled_rectangle(ResolutionButton_x, ResolutionButton_y,
            ResolutionButton_x + ResolutionButton_w, ResolutionButton_y + ResolutionButton_h, resolution_color);
        al_draw_rectangle(ResolutionButton_x, ResolutionButton_y,
            ResolutionButton_x + ResolutionButton_w, ResolutionButton_y + ResolutionButton_h,
            al_map_rgb(255, 255, 255), 2);
        al_draw_text(fontSmall, al_map_rgb(255, 255, 255),
            ResolutionButton_x + (ResolutionButton_w / 2.0), ResolutionButton_y + 4,
            ALLEGRO_ALIGN_CENTRE, "ROZDZIELCZOSC");

        // Przycisk resetu
        ALLEGRO_COLOR reset_color = reset_hovered ? al_map_rgb(255, 100, 100) : al_map_rgb(0, 0, 0);
        al_draw_filled_rectangle(ResetButton_x, ResetButton_y,
            ResetButton_x + ResetButton_w, ResetButton_y + ResetButton_h, reset_color);
        al_draw_rectangle(ResetButton_x, ResetButton_y,
            ResetButton_x + ResetButton_w, ResetButton_y + ResetButton_h,
            al_map_rgb(255, 255, 255), 2);
        al_draw_text(fontSmall, al_map_rgb(255, 255, 255),
            ResetButton_x + (ResetButton_w / 2.0), ResetButton_y + 4,
            ALLEGRO_ALIGN_CENTRE, "RESET TIMERA");

        // Workspace i timer
        al_draw_filled_rectangle(WorkspacePlace_x, WorkspacePlace_y,
            WorkspacePlace_x + WorkspacePlace_w, WorkspacePlace_y + WorkspacePlace_h,
            al_map_rgb(190, 190, 190));
        al_draw_text(fontLarge, al_map_rgb(255, 215, 0),
            Timer_x, Timer_y, ALLEGRO_ALIGN_CENTRE, timerText.c_str());

        al_flip_display();
    }

    void cleanup() {
        al_destroy_timer(timer);
        if (fontSmall) al_destroy_font(fontSmall);
        if (fontLarge && fontLarge != fontSmall) al_destroy_font(fontLarge);
        if (queue) al_destroy_event_queue(queue);
        if (display) al_destroy_display(display);
    }
};

int main() {
    Engine engine;
    engine.run();
    return 0;
}
