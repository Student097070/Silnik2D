#include "Include.h"
#include "PrimitiveRenderer.h"
#include "STR.h"
#include "Button.h"
#include "Point2D.h"
#include "LineSegment.h"

Button ResolutionButton(0, 0, 100, 20, "Rozdzielczosc", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button ResetButton(110, 0, 100, 20, "RESET TIMERA", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button CircleButton(220, 0, 100, 20, "CIRCLE", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button Point2DButton(330, 0, 100, 20, "Point2D", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button CleanButton(660, 0, 100, 20, "CLEAN", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button RectangleButton(0, 30, 100, 20, "RECTANGLE", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button TriangleButton(110, 30, 100, 20, "TRIANGLE", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button LineButton(220, 30, 100, 20, "LINE", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button CircleCustomButton(330, 30, 100, 20, "Circle v2", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button ElipseCustomButton(440, 30, 100, 20, "Elipse", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button PolygonButton(0, 60, 100, 20, "Polygon", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));


// Klasa silnika programu
class Engine {
private:
  
    // Tablica dostępncyh rozdzielczości
    Resolution resolutions[5] = {
        {800, 640, 400, 300},
        {1024, 768, 500, 350},
        {1152, 864, 600, 400},
        {1280, 720, 1000, 500},
        {1920, 1080, 1200, 750}
    };
   
    time_t start_time = time(nullptr);  // Czas rozpoczęcia czasomierza  


private:
    // Inicjalizacja pliku log z błędami
    void initLog() {
        ofstream errorFile("error_log.txt");
        if (errorFile.is_open()) {
            errorFile << "=== Log błędów programu ===" << endl;
            errorFile.close();
        }
    }
public:
	bool CircleDrawingMode = false;
	bool PointDrawingMode = false;
    bool RectangleDrawingMode = false;
    bool TriangleDrawingMode = false;
    bool Circle2DrawingMode = false;
    bool ElipseDrawingMode = false;
    bool PolygonDrawingMode = false;
	bool LineDrawingMode = false;
    
	vector<Point2D> points; // Przechowywanie punktow Point2D
    vector<CircleData> circles; // Przechowywanie punktow Circle
	vector<RectangleData> rectangles; // Przechowywanie prostokatow Rectangle
	vector<TriangleData> triangles; // Przechowywanie trojkątów Triangle
    vector<Circle2Data> circles2;
    vector<ElipseData> elipses;
    vector<vector<PolygonPoint>> polygons;
    vector<PolygonPoint> punktyWielokata;

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

    void clear() { // czyszczenie mapy
        points.clear(); 
		circles.clear();
        circles2.clear();
        elipses.clear();
        polygons.clear();
        punktyWielokata.clear();
    }


    // Rysowanie interfejsu
    void draw(ALLEGRO_EVENT& ev) {
        al_clear_to_color(al_map_rgb(30, 30, 30));

        ResolutionButton.draw();
        ResetButton.draw();
        CircleButton.draw();
		Point2DButton.draw();
		CleanButton.draw();
		RectangleButton.draw();
		TriangleButton.draw();
		LineButton.draw();
        CircleCustomButton.draw();
        ElipseCustomButton.draw();
        PolygonButton.draw();



        //punkt
        if (PointDrawingMode) {
            Point2DButton.ButtonCollor = al_map_rgb(0, 255, 0);
            Point2DButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            Point2DButton.ButtonCollor = al_map_rgb(255, 0, 0);
            Point2DButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        //kolo
        if (CircleDrawingMode) {
            CircleButton.ButtonCollor = al_map_rgb(0, 255, 0);
            CircleButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            CircleButton.ButtonCollor = al_map_rgb(255, 0, 0);
            CircleButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        //linia
        if (LineDrawingMode) {
            LineButton.ButtonCollor = al_map_rgb(0, 255, 0);
            LineButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            LineButton.ButtonCollor = al_map_rgb(255, 0, 0);
            LineButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        //kwadrat
        /*if (RectangleDrawingMode) {
            RectangleButton.ButtonCollor = al_map_rgb(0, 255, 0); 
            RectangleButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            RectangleButton.ButtonCollor = al_map_rgb(255, 0, 0);
            RectangleButton.TextCollor = al_map_rgb(255, 255, 255);
        }*/

		//trojkat
        /*if (TriangleDrawingMode) {
            TriangleButton.ButtonCollor = al_map_rgb(0, 255, 0);
            TriangleButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            TriangleButton.ButtonCollor = al_map_rgb(255, 0, 0);
            TriangleButton.TextCollor = al_map_rgb(255, 255, 255);
        }*/

        if (Circle2DrawingMode) {
            CircleCustomButton.ButtonCollor = al_map_rgb(0, 255, 0);
            CircleCustomButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            CircleCustomButton.ButtonCollor = al_map_rgb(255, 0, 0);
            CircleCustomButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        if (ElipseDrawingMode) {
            ElipseCustomButton.ButtonCollor = al_map_rgb(0, 255, 0);
            ElipseCustomButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            ElipseCustomButton.ButtonCollor = al_map_rgb(255, 0, 0);
            ElipseCustomButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        if (PolygonDrawingMode) {
            PolygonButton.ButtonCollor = al_map_rgb(0, 255, 0);
            PolygonButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            PolygonButton.ButtonCollor = al_map_rgb(255, 0, 0);
            PolygonButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        // Rysowanie obszaru roboczego i czasomierza
        al_draw_filled_rectangle(WorkspacePlace_x, WorkspacePlace_y,
            WorkspacePlace_x + WorkspacePlace_w, WorkspacePlace_y + WorkspacePlace_h,
            al_map_rgb(190, 190, 190));

        

        for (auto& p : points)
            p.DisplayPoint();

        for (auto& c : circles) {
            PrimitiveRenderer renderer(c.color);
            renderer.circle(c.x, c.y, c.r, true, 2.0);
        }

        for (auto& c : circles2) {
            PrimitiveRenderer renderer(c.color);
            renderer.circlecustom(c.x0, c.y0, c.R);
        }

        for (auto& c : elipses) {
            PrimitiveRenderer renderer(c.color);
            renderer.elipsecustom(c.x0, c.y0, c.Rx, c.Ry);
        }

        if (PolygonDrawingMode && !punktyWielokata.empty()) {
            for (auto& p : punktyWielokata) {
                al_draw_filled_circle(p.x, p.y, 3, al_map_rgb(255, 0, 0));
            }
            for (size_t i = 0; i + 1 < punktyWielokata.size(); ++i) {
                al_draw_line(punktyWielokata[i].x, punktyWielokata[i].y,
                    punktyWielokata[i + 1].x, punktyWielokata[i + 1].y,
                    al_map_rgb(180, 0, 0), 1.0f);
            }
            for (auto& p : punktyWielokata) {
                al_draw_filled_circle(p.x, p.y, 3, al_map_rgb(255, 0, 0));
            }

        }

        for (auto& c : polygons) {
            PrimitiveRenderer renderer(al_map_rgb(255, 0, 0));
            renderer.polygon(c);
        }
      

        // Czasomierz
        time_t elapsed = time(nullptr) - start_time;
        string timerText = to_string(elapsed);
        al_draw_text(fontLarge, al_map_rgb(255, 215, 0),
            Timer_x, Timer_y, ALLEGRO_ALIGN_CENTRE, timerText.c_str());
        al_flip_display();
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
            draw(ev);
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
            handleMouseMove(ev.mouse.x, ev.mouse.y);
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

            // Kliknięcie w przyciski:
            if (ResolutionButton.hovered(ev.mouse.x, ev.mouse.y))
                changeResolution();

            else if (ResetButton.hovered(ev.mouse.x, ev.mouse.y))
                resetTimer();

			else if (CleanButton.hovered(ev.mouse.x, ev.mouse.y))
				clear();

            else if (CircleButton.hovered(ev.mouse.x, ev.mouse.y)) {
                CircleDrawingMode = !CircleDrawingMode;
            }

            else if (RectangleButton.hovered(ev.mouse.x, ev.mouse.y)) {
                RectangleDrawingMode = !RectangleDrawingMode;
            }

            else if (TriangleButton.hovered(ev.mouse.x, ev.mouse.y)) {
                TriangleDrawingMode = !TriangleDrawingMode;
            }

            else if (Point2DButton.hovered(ev.mouse.x, ev.mouse.y)) {
                PointDrawingMode = !PointDrawingMode; // przelaczanie trybu rysowania 
            }

            else if (CircleCustomButton.hovered(ev.mouse.x, ev.mouse.y)) {
                Circle2DrawingMode = !Circle2DrawingMode;
            }

            else if (ElipseCustomButton.hovered(ev.mouse.x, ev.mouse.y)) {
                ElipseDrawingMode = !ElipseDrawingMode;
            }

            else if (PolygonButton.hovered(ev.mouse.x, ev.mouse.y)) {
                PolygonDrawingMode = !PolygonDrawingMode;
            }
            // Jeśli kliknięto poza przyciskami i tryb rysowania jest aktywny:
            else if (PointDrawingMode) {
                Point2D newPoint(ev.mouse.x, ev.mouse.y, al_map_rgb(255, 255, 255));
                points.push_back(newPoint);  // zapamiętaj punkt
            }

            else if (CircleDrawingMode) {
                CircleData newCircle;
                newCircle.x = ev.mouse.x;
                newCircle.y = ev.mouse.y;
                newCircle.r = 40;
                newCircle.color = al_map_rgb(0, 255, 0);
                circles.push_back(newCircle);
            }
            else if (Circle2DrawingMode) {
                Circle2Data newCircle;
                newCircle.x0 = ev.mouse.x;
                newCircle.y0 = ev.mouse.y;
                newCircle.R = 40;
                newCircle.color = al_map_rgb(0, 255, 255);
                circles2.push_back(newCircle);
            }
            else if (ElipseDrawingMode) {
                ElipseData newElipse;
                newElipse.x0 = ev.mouse.x;
                newElipse.y0 = ev.mouse.y;
                newElipse.Rx = 40;
                newElipse.Ry = 20;
                newElipse.color = al_map_rgb(0, 0, 255);
                elipses.push_back(newElipse);
            }
            else if (PolygonDrawingMode) {
                PolygonPoint newPoly;
                newPoly.x = ev.mouse.x;
                newPoly.y = ev.mouse.y;
                punktyWielokata.push_back(newPoly);
            }
        }

        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            handleKey(ev.keyboard.keycode);
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                polygons.push_back(punktyWielokata);
                punktyWielokata.clear();
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            handleResize();
    }

    // Obsługa ruchu myszy
    void handleMouseMove(int x, int y) {
        resolution_hovered = ResolutionButton.hovered(x, y);
        reset_hovered = ResetButton.hovered(x, y);
        prim_hovered = CircleButton.hovered(x, y);
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

    void primitive() {
        PrimitiveRenderer p(al_map_rgb(0, 255, 0));
        p.circle(200,200, 100, false, 3.5);
        al_flip_display();
    }

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
        logError("Silnik uruchomiony pomyslnie");
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
        logError("Silnik zakonczył dzialanie pomyslnie");
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
    return 0;
}