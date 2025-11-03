#include "Include.h"

// Klasa silnika programu
class Engine {
private:
    // Komponenty biblioteki Allegro
    ALLEGRO_DISPLAY* display = nullptr;
    ALLEGRO_EVENT_QUEUE* queue = nullptr;
    ALLEGRO_TIMER* timer = nullptr;
    ALLEGRO_FONT* fontSmall = nullptr;
    ALLEGRO_FONT* fontLarge = nullptr;

    // Tablica dostępncyh rozdzielczości
    Resolution resolutions[5] = {
        {800, 640, 400, 300},
        {1024, 768, 500, 350},
        {1152, 864, 600, 400},
        {1280, 720, 1000, 500},
        {1920, 1080, 1200, 750}
    };
    int current_index = 0;            //Aktualna rozdzielczość 

    bool running = true;              // Stan działania aplikacji
    bool fullscreen = false;          // Czy jest włączony tryb pełnoekranowy
    bool resolution_hovered = false;  // Czy myszka jest nad przyciskiem rozdzielczości
    bool reset_hovered = false;       // Czy myszka jest nad przyciskiem resetu
    bool prim_hovered = false;

    // Pozycje i rozmiary elementów interfejsu
    float ResolutionButton_x = 0, ResolutionButton_y = 0;           // Pozycja przycisku zmiany rozdzielczości
    float ResolutionButton_w = 100, ResolutionButton_h = 20;        // Rozmiar przycisku zmiany rozdzielczości
    float ResetButton_x = 110, ResetButton_y = 0;                   // Pozycja przycisku resetu czasomierza  
    float ResetButton_w = 100, ResetButton_h = 20;                // Rozmiar przycisku resetu czasomierza
    float PrimButton_x = 220, PrimButton_y = 0;                  
    float PrimButton_w = 100, PrimButton_h = 20;
    float WorkspacePlace_x, WorkspacePlace_y, WorkspacePlace_w, WorkspacePlace_h;  // Pozycja i rozmiar obszaru roboczego   
    float Timer_x, Timer_y;						                	// Pozycja wyświetlania czasomierza

    time_t start_time = time(nullptr);  // Czas rozpoczęcia czasomierza  

public:
    // Konstruktor inicjalizujący silnik
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

    // Czyszczenie zasobów
    ~Engine() {
        cleanup();
    }

    // Główna pętla programu
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
    // Inicjalizacja pliku log z błędami
    void initLog() {
        ofstream errorFile("error_log.txt");
        if (errorFile.is_open()) {
            errorFile << "=== Log błędów programu ===" << endl;
            errorFile.close();
        }
    }

    // Zapisywanie komunikatu z błędami
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

    // Inicjalizacja biblioteki Allegro i jej dodatków
    bool initAllegro() {
        if (!al_init()) { logError("Błąd inicjalizacji Allegro!"); return false; }
        if (!al_init_font_addon()) { logError("Błąd inicjalizacji fontów!"); return false; }
        if (!al_init_ttf_addon()) { logError("Błąd inicjalizacji TTF!"); return false; }
        if (!al_init_primitives_addon()) { logError("Błąd inicjalizacji primitives!"); return false; }
        if (!al_install_mouse()) { logError("Błąd inicjalizacji myszy!"); return false; }
        if (!al_install_keyboard()) { logError("Błąd inicjalizacji klawiatury!"); return false; }

        return true;
    }

    // Tworzenie głównego okna aplikacji
    void createDisplay() {
        al_set_new_display_flags(ALLEGRO_RESIZABLE);
        display = al_create_display(resolutions[current_index].w, resolutions[current_index].h);
        if (!display) { logError("Błąd tworzenia display!"); exit(-1); }
        al_set_window_title(display, "Engine Allegro");
        queue = al_create_event_queue();
        if (!queue) { logError("Błąd tworzenia kolejki zdarzeń!"); exit(-1); }
    }

    // Ładowanie czcionek
    void loadFonts() {
        fontSmall = al_load_ttf_font("Arial.ttf", 11, 0);
        if (!fontSmall) {
            logError("Błąd ładowania czcionki Arial.ttf (11px)!");
            fontSmall = al_create_builtin_font();
        }
        fontLarge = al_load_ttf_font("Arial.ttf", 24, 0);
        if (!fontLarge) fontLarge = fontSmall;
    }

    // Tworzenie czasomierza
    void createTimer() {
        timer = al_create_timer(1.0 / 144.0);
        if (!timer) { logError("Błąd tworzenia timera!"); exit(-1); }
    }

    // Rejestrowanie źródeł zdarzeń
    void registerEventSources() {
        al_register_event_source(queue, al_get_display_event_source(display));
        al_register_event_source(queue, al_get_timer_event_source(timer));
        al_register_event_source(queue, al_get_mouse_event_source());
        al_register_event_source(queue, al_get_keyboard_event_source());
    }

    // Ustawianie obszaru roboczego
    void setupWorkspace() {
        WorkspacePlace_w = resolutions[current_index].workspace_w;
        WorkspacePlace_h = resolutions[current_index].workspace_h;
        WorkspacePlace_x = (resolutions[current_index].w / 2.0f) - (WorkspacePlace_w / 2.0f);
        WorkspacePlace_y = (resolutions[current_index].h / 2.0f) - (WorkspacePlace_h / 2.0f);
        // Pozycja czasomierza
        Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
        Timer_y = WorkspacePlace_y - 40;
    }

    // Centrowanie okna na ekranie
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

    // Resetowanie czasomierza
    void resetTimer() {
        start_time = time(nullptr);
    }

    // Obsługa zdarzeń
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

    // Obsługa ruchu myszy
    void handleMouseMove(int x, int y) {
        resolution_hovered =
            (x >= ResolutionButton_x && x <= ResolutionButton_x + ResolutionButton_w &&
                y >= ResolutionButton_y && y <= ResolutionButton_y + ResolutionButton_h);
        reset_hovered =
            (x >= ResetButton_x && x <= ResetButton_x + ResetButton_w &&
                y >= ResetButton_y && y <= ResetButton_y + ResetButton_h);
        prim_hovered =
            (x >= PrimButton_x && x <= PrimButton_x + PrimButton_w &&
                y >= PrimButton_y && y <= PrimButton_y + PrimButton_h);
    }

    // Obsługa kliknięcia myszy
    void handleMouseClick() {
        if (resolution_hovered) changeResolution();
        else if (reset_hovered) resetTimer();
        //else if (prim_hovered) primitive();
    }

    // Obsługa klawiszy
    void handleKey(int key) {
        switch (key) {
        case ALLEGRO_KEY_ESCAPE: running = false; break;
        case ALLEGRO_KEY_R: resetTimer(); break;
        case ALLEGRO_KEY_F: toggleFullscreen(); break;
        }
    }

    // Obsługa zmiany rozmiaru okna
    void handleResize() {
        al_acknowledge_resize(display);
        int new_w = al_get_display_width(display);
        int new_h = al_get_display_height(display);
        WorkspacePlace_x = (new_w / 2.0f) - (WorkspacePlace_w / 2.0f);
        WorkspacePlace_y = (new_h / 2.0f) - (WorkspacePlace_h / 2.0f);
        Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
        Timer_y = WorkspacePlace_y - 40;
    }

    /*void primitive() {
        PrimitiveRenderer p(al_map_rgb(0, 255, 0));
        p.circle(200,200, 100, true, 3.5);
        al_flip_display();
    }*/
    // Zmiana rozdzielczości okna
    void changeResolution() {
        current_index = (current_index + 1) % 5;
        al_resize_display(display, resolutions[current_index].w, resolutions[current_index].h);
        setupWorkspace();
        centerWindow();
    }

    // Przełączanie trybu pełnoekranowego
    void toggleFullscreen() {
        fullscreen = !fullscreen;
        if (!al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen))
            logError("Błąd przełączania pełnego ekranu!");
    }

    // Rysowanie interfejsu
    void draw() {
        // Obliczanie czasu czasomierza
        time_t elapsed = time(nullptr) - start_time;
        string timerText = to_string(elapsed);

        // Wyczyszczenie ekranu
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

        // Przycisk resetu czasomierza
        ALLEGRO_COLOR reset_color = reset_hovered ? al_map_rgb(255, 100, 100) : al_map_rgb(0, 0, 0);
        al_draw_filled_rectangle(ResetButton_x, ResetButton_y,
            ResetButton_x + ResetButton_w, ResetButton_y + ResetButton_h, reset_color);
        al_draw_rectangle(ResetButton_x, ResetButton_y,
            ResetButton_x + ResetButton_w, ResetButton_y + ResetButton_h,
            al_map_rgb(255, 255, 255), 2);
        al_draw_text(fontSmall, al_map_rgb(255, 255, 255),
            ResetButton_x + (ResetButton_w / 2.0), ResetButton_y + 4,
            ALLEGRO_ALIGN_CENTRE, "RESET TIMERA");

        ALLEGRO_COLOR prim_color = prim_hovered ? al_map_rgb(0, 120, 255) : al_map_rgb(0, 0, 0);
        al_draw_filled_rectangle(PrimButton_x, PrimButton_y,
            PrimButton_x + PrimButton_w, PrimButton_y + PrimButton_h, prim_color);
        al_draw_rectangle(ResolutionButton_x, ResolutionButton_y,
            PrimButton_x + PrimButton_w, PrimButton_y + PrimButton_h,
            al_map_rgb(255, 255, 255), 2);
        al_draw_text(fontSmall, al_map_rgb(255, 255, 255),
            PrimButton_x + (PrimButton_w / 2.0), PrimButton_y + 4,
            ALLEGRO_ALIGN_CENTRE, "Primus");

        // Rysowanie obszaru roboczego i czasomierza
        al_draw_filled_rectangle(WorkspacePlace_x, WorkspacePlace_y,
            WorkspacePlace_x + WorkspacePlace_w, WorkspacePlace_y + WorkspacePlace_h,
            al_map_rgb(190, 190, 190));
        al_draw_text(fontLarge, al_map_rgb(255, 215, 0),
            Timer_x, Timer_y, ALLEGRO_ALIGN_CENTRE, timerText.c_str());
        if (prim_hovered) {
            PrimitiveRenderer p(al_map_rgb(0, 255, 0));
            p.circle(200, 200, 100, true, 3.5);
        }
        // Wyświetlenie zmian
        al_flip_display();

    }

    // Czyszczenie zasobów
    void cleanup() {
        al_destroy_timer(timer);
        if (fontSmall) al_destroy_font(fontSmall);
        if (fontLarge && fontLarge != fontSmall) al_destroy_font(fontLarge);
        if (queue) al_destroy_event_queue(queue);
        if (display) al_destroy_display(display);
    }
};

// Główna funkcja programu
int main() {
    Engine engine;      // Utworzenie instancji silnika
    engine.run();       // Uruchomienie głównej pętli
    //PrimitiveRenderer p(al_map_rgb(0, 255, 0));
    //p.circle(200,200, 100, true, 3.5);

    return 0;
}