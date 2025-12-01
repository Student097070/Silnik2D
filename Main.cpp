/**
 * @file Main.cpp
 * @brief Główny plik źródłowy programu, zawierający klasę Engine i punkt wejścia.
 * @details
 * - Implementuje główną pętlę aplikacji, zarządza przyciskami i trybami rysowania.
 * - Obsługuje zdarzenia klawiatury, myszy oraz zmiany rozdzielczości.
 * - Zawiera kontenery dla różnych typów obiektów graficznych (punkty, trójkąty, koła, elipsy, itp.).
 * - Używa Allegro do zarządzania oknem, timerem i renderowaniem.
 */

#include "Include.h"
#include "PrimitiveRenderer.h"
#include "STR.h"
#include "Button.h"
#include "Point2D.h"
#include "LineSegment.h"
#include "ShapeObject.h"
#include "ShapeCircle.h"
#include "ShapePolygon.h"
#include "Player.h"
#include "TriangleObject.h"
#include "RectangleObject.h"
#include "CircleObject.h"
#include "EllipseObject.h"

 /// Przyciski interfejsu użytkownika

Button ResolutionButton(0, 0, 100, 20, "Rozdzielczosc", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button ResetButton(110, 0, 100, 20, "RESET TIMERA", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));

Button TriangleTransformButton(220, 0, 100, 20, "Triange transform", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button CircleTransformButton(330, 0, 100, 20, "Circle transform", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button RectangleTransformButton(440, 0, 100, 20, "Rectangle transform", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button EllipseleTransformButton(550, 0, 100, 20, "Elipse transform", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));

Button CleanButton(660, 0, 100, 20, "CLEAN", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button RectangleButton(0, 30, 100, 20, "RECTANGLE", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button TriangleButton(110, 30, 100, 20, "TRIANGLE", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button CircleCustomButton(330, 30, 100, 20, "Circle", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button ElipseCustomButton(440, 30, 100, 20, "Elipse", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button Point2DButton(550, 30, 100, 20, "Point2D", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));

Button PolygonButton(0, 60, 100, 20, "Polygon", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button PolylineButton(220, 30, 100, 20, "Polyline", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
Button Fill1Button(110, 60, 100, 20, "FILL boundary", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255)); 
Button Fill2Button(220, 60, 100, 20, "FILL flood", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));

/**
 * @class Engine
 * @brief Główna klasa silnika aplikacji, zarządzająca inicjalizacją, pętlą główną i logiką programu.
 */
class Engine {
private:

    // Tablica dostępnych rozdzielczości
    Resolution resolutions[5] = {
        {800, 640, 400, 300},
        {1024, 768, 500, 350},
        {1152, 864, 600, 400},
        {1280, 720, 1000, 500},
        {1920, 1080, 1200, 750}
    };

    time_t start_time = time(nullptr);  // Czas rozpoczęcia czasomierza  

    // Inicjalizacja pliku log z błędami
    void initLog() {
        ofstream errorFile("error_log.txt");
        if (errorFile.is_open()) {
            errorFile << "=== Log błędów programu ===" << endl;
            errorFile.close();
        }
    }

public:
    /// Flagi trybów rysowania
    bool TriangleTransform = false;
    bool RectangleTransform = false;
    bool CircleTransform = false;
    bool ElipseTransform = false;
    bool PointDrawingMode = false;
    bool RectangleDrawingMode = false;
    bool TriangleDrawingMode = false;
    bool Circle2DrawingMode = false;
    bool ElipseDrawingMode = false;
    bool PolygonDrawingMode = false;
    bool LineDrawingMode = false;
    bool PolylineDrawingMode = false;
    bool Fill1DrawingMode = false;
    bool Fill2DrawingMode = false;

    /// Kontenery dla obiektów graficznych
    vector<Point2D> points;

    vector<unique_ptr<TriangleObject>> triangles;
    vector<unique_ptr<CircleObject>> circles2;
    vector<unique_ptr<EllipseObject>> elipses;
    vector<unique_ptr<RectangleObject>> rectangles;

    vector<vector<PolygonPoint>> polygons;
    vector<PolygonPoint> punktyWielokata;
    vector<Point2D> polylinePoints;
    vector<LineSegment> lineSegments;
    vector<pair<float, float>> tempTrianglePoints; 
    vector<pair<float, float>> tempRectanglePoints;
    vector<FillPoint> fill1points;
    vector<FillPoint> fill2points;

    vector<unique_ptr<ShapeObject>> shapes;
    unique_ptr<Player> player;
    

    /**
     * @brief Zapisuje komunikat błędu do pliku log.
     * @param errorMessage Tekst błędu do zapisania.
     */
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
        al_init_image_addon();

        return true;
    }

    // Tworzenie głównego okna aplikacji
    void createDisplay() const {
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
        timer = al_create_timer(1.0 / 60.0);
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
    void setupWorkspace()const {
        WorkspacePlace_w = resolutions[current_index].workspace_w;
        WorkspacePlace_h = resolutions[current_index].workspace_h;
        WorkspacePlace_x = (resolutions[current_index].w / 2.0f) - (WorkspacePlace_w / 2.0f);
        WorkspacePlace_y = (resolutions[current_index].h / 2.0f) - (WorkspacePlace_h / 2.0f);
        Timer_x = WorkspacePlace_x + (WorkspacePlace_w / 2.0f);
        Timer_y = WorkspacePlace_y - 40;
    }

    // Centrowanie okna na ekranie
    void centerWindow()const {
        ALLEGRO_MONITOR_INFO info;
        if (al_get_monitor_info(0, &info)) {
            int screen_w = info.x2 - info.x1;
            int screen_h = info.y2 - info.y1;
            int win_x = (screen_w - resolutions[current_index].w) / 2;
            int win_y = (screen_h - resolutions[current_index].h) / 2;
            al_set_window_position(display, win_x, win_y);
        }
    }

    void clear() {
        points.clear();
        circles2.clear();
        elipses.clear();
        polygons.clear();
        triangles.clear();
        tempTrianglePoints.clear();
        rectangles.clear();
        tempRectanglePoints.clear();
        punktyWielokata.clear();
        polylinePoints.clear();
        lineSegments.clear();
        fill1points.clear();
        fill2points.clear();
    }

    // Rysowanie interfejsu
    void draw(ALLEGRO_EVENT& ev) {
        al_clear_to_color(al_map_rgb(30, 30, 30));

        // Rysowanie przycisków
        ResolutionButton.draw();
        ResetButton.draw();

        TriangleTransformButton.draw();
		RectangleTransformButton.draw();
		EllipseleTransformButton.draw();
		CircleTransformButton.draw();
        
        Point2DButton.draw();
        CleanButton.draw();
        RectangleButton.draw();
        TriangleButton.draw();
        CircleCustomButton.draw();
        ElipseCustomButton.draw();
        PolygonButton.draw();
        PolylineButton.draw();
        Fill1Button.draw();
        Fill2Button.draw();

        // Zmiana kolorów przycisków w zależności od aktywnego trybu
        if (PointDrawingMode) {
            Point2DButton.ButtonCollor = al_map_rgb(0, 255, 0);
            Point2DButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            Point2DButton.ButtonCollor = al_map_rgb(255, 0, 0);
            Point2DButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        if (TriangleTransform) {
            TriangleTransformButton.ButtonCollor = al_map_rgb(0, 255, 0);
            TriangleTransformButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            TriangleTransformButton.ButtonCollor = al_map_rgb(255, 0, 0);
            TriangleTransformButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        //kwadrat
        if (RectangleDrawingMode) {
            RectangleButton.ButtonCollor = al_map_rgb(0, 255, 0);
            RectangleButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            RectangleButton.ButtonCollor = al_map_rgb(255, 0, 0);
            RectangleButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        if(RectangleTransform) {
            RectangleTransformButton.ButtonCollor = al_map_rgb(0, 255, 0);
            RectangleTransformButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            RectangleTransformButton.ButtonCollor = al_map_rgb(255, 0, 0);
            RectangleTransformButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        //trojkat
        if (TriangleDrawingMode) {
            TriangleButton.ButtonCollor = al_map_rgb(0, 255, 0);
            TriangleButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            TriangleButton.ButtonCollor = al_map_rgb(255, 0, 0);
            TriangleButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        if(TriangleTransform) {
            TriangleTransformButton.ButtonCollor = al_map_rgb(0, 255, 0);
            TriangleTransformButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            TriangleTransformButton.ButtonCollor = al_map_rgb(255, 0, 0);
            TriangleTransformButton.TextCollor = al_map_rgb(255, 255, 255);
		}

        if (Circle2DrawingMode) {
            CircleCustomButton.ButtonCollor = al_map_rgb(0, 255, 0);
            CircleCustomButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            CircleCustomButton.ButtonCollor = al_map_rgb(255, 0, 0);
            CircleCustomButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        if(CircleTransform) {
            CircleTransformButton.ButtonCollor = al_map_rgb(0, 255, 0);
            CircleTransformButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            CircleTransformButton.ButtonCollor = al_map_rgb(255, 0, 0);
            CircleTransformButton.TextCollor = al_map_rgb(255, 255, 255);
		}

        if (ElipseDrawingMode) {
            ElipseCustomButton.ButtonCollor = al_map_rgb(0, 255, 0);
            ElipseCustomButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            ElipseCustomButton.ButtonCollor = al_map_rgb(255, 0, 0);
            ElipseCustomButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        if(ElipseTransform) {
            EllipseleTransformButton.ButtonCollor = al_map_rgb(0, 255, 0);
            EllipseleTransformButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            EllipseleTransformButton.ButtonCollor = al_map_rgb(255, 0, 0);
            EllipseleTransformButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        if (PolygonDrawingMode) {
            PolygonButton.ButtonCollor = al_map_rgb(0, 255, 0);
            PolygonButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            PolygonButton.ButtonCollor = al_map_rgb(255, 0, 0);
            PolygonButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        if (PolylineDrawingMode) {
            PolylineButton.ButtonCollor = al_map_rgb(0, 255, 0);
            PolylineButton.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            PolylineButton.ButtonCollor = al_map_rgb(255, 0, 0);
            PolylineButton.TextCollor = al_map_rgb(255, 255, 255);
        }

        if (Fill1DrawingMode) {
            Fill1Button.ButtonCollor = al_map_rgb(0, 255, 0);
            Fill1Button.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            Fill1Button.ButtonCollor = al_map_rgb(255, 0, 0);
            Fill1Button.TextCollor = al_map_rgb(255, 255, 255);
        }
        if (Fill2DrawingMode) {
            Fill2Button.ButtonCollor = al_map_rgb(0, 255, 0);
            Fill2Button.TextCollor = al_map_rgb(0, 0, 0);
        }
        else {
            Fill2Button.ButtonCollor = al_map_rgb(255, 0, 0);
            Fill2Button.TextCollor = al_map_rgb(255, 255, 255);
        }


        // Rysowanie obszaru roboczego
        al_draw_filled_rectangle(WorkspacePlace_x, WorkspacePlace_y,
            WorkspacePlace_x + WorkspacePlace_w, WorkspacePlace_y + WorkspacePlace_h,
            al_map_rgb(190, 190, 190));

        const int MAX_POINTS = 1000;
        if (points.size() > MAX_POINTS) {
            points.erase(points.begin(), points.begin() + (points.size() - MAX_POINTS));
        }

        // Rysowanie wszystkich obiektów
        for (auto& p : points)
            p.DisplayPoint();

        for (auto& t : triangles) {
            PrimitiveRenderer renderer(t->data.color);
            renderer.triangle(t->data.x0, t->data.y0, t->data.x1, t->data.y1, t->data.x2, t->data.y2);
        }

        for (auto& c : rectangles) {
            PrimitiveRenderer renderer(c->data.color);
            renderer.rectangle(c->data.x0, c->data.y0, c->data.x1, c->data.y1);
        }
        
        
        // Rysowanie trwającej linii łamanej
        if (PolylineDrawingMode && !polylinePoints.empty()) {
            for (auto& p : polylinePoints) {
                al_draw_filled_circle(p.x, p.y, 3, al_map_rgb(0, 255, 0));
            }
            PrimitiveRenderer renderer(al_map_rgb(0, 255, 0));
            renderer.polyline(polylinePoints);
        }
        //ALLEGRO_BITMAP* bb = al_get_backbuffer(display);
        //al_lock_bitmap(bb, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
        // Rysowanie zakończonych wielokątów
        for (auto& c : polygons) {
            PrimitiveRenderer renderer(al_map_rgb(255, 0, 0));
            renderer.polygon(c);
        }
        for (auto& c : circles2) {
            PrimitiveRenderer renderer(c->data.color);
            renderer.circlecustom(c->data.x0, c->data.y0, c->data.R);
        }

        for (auto& c : elipses) {
            PrimitiveRenderer renderer(c->data.color);
            renderer.elipsecustom(c->data.x0, c->data.y0, c->data.Rx, c->data.Ry);
        }
        for (auto& c : fill1points) {
            PrimitiveRenderer renderer(al_map_rgb(255, 0, 0));
            renderer.boundaryFill(c.x, c.y, al_map_rgb(0, 255, 0), al_map_rgb(255, 0, 0));
        }
        for (auto& c : fill2points) {
            PrimitiveRenderer renderer(al_map_rgb(255, 0, 0));
            renderer.floodFill(c.x, c.y, al_map_rgb(0, 255, 0), al_map_rgb(190, 190, 190));
        }


       
        for (auto& s : shapes) s->draw();
        

        //al_unlock_bitmap(bb);

        if (player) player->draw();

        if (TriangleDrawingMode && !tempTrianglePoints.empty()) {
            for (auto& p : tempTrianglePoints) {
                al_draw_filled_circle(p.first, p.second, 5, al_map_rgb(0, 0, 255));
            }

            if (tempTrianglePoints.size() == 2) {
                al_draw_line(tempTrianglePoints[0].first, tempTrianglePoints[0].second,
                    tempTrianglePoints[1].first, tempTrianglePoints[1].second,
                    al_map_rgb(0, 0, 255), 2.0f);
            }
        }

        if (RectangleDrawingMode && !tempRectanglePoints.empty()) {
            for (auto& p : tempRectanglePoints) {
                al_draw_filled_circle(p.first, p.second, 5, al_map_rgb(255, 0, 255));
            }

            if (tempRectanglePoints.size() == 2) {
                al_draw_rectangle(tempRectanglePoints[0].first, tempRectanglePoints[0].second,
                    tempRectanglePoints[1].first, tempRectanglePoints[1].second,
                    al_map_rgb(255, 0, 255), 2.0f);
            }
        }

        // Rysowanie trwającego wielokąta
        if (PolygonDrawingMode && !punktyWielokata.empty()) {
            for (auto& p : punktyWielokata) {
                al_draw_filled_circle(p.x, p.y, 3, al_map_rgb(255, 0, 0));
            }
            for (size_t i = 0; i + 1 < punktyWielokata.size(); ++i) {
                al_draw_line(punktyWielokata[i].x, punktyWielokata[i].y,
                    punktyWielokata[i + 1].x, punktyWielokata[i + 1].y,
                    al_map_rgb(180, 0, 0), 1.0f);
            }
        }




        // Czasomierz
        time_t elapsed = time(nullptr) - start_time;
        string timerText = to_string(elapsed);
        al_draw_text(fontLarge, al_map_rgb(255, 215, 0),
            Timer_x, Timer_y, ALLEGRO_ALIGN_CENTRE, timerText.c_str());
        al_flip_display();
    }

    void resetTimer() {
        start_time = time(nullptr);
    }

    // Obsługa zdarzeń
    void handleEvent(ALLEGRO_EVENT& ev) {
		
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_KEYBOARD_STATE keyState;
            al_get_keyboard_state(&keyState);
            float dt = 1.0f / 60.0f;   // stały czas kroku
            if (player) player->update(dt);
            draw(ev);
          /*  if (player) {
                if (al_key_down(&keyState, ALLEGRO_KEY_UP)) {
                    player->pos.y -= 1;
                }
                if (al_key_down(&keyState, ALLEGRO_KEY_DOWN)) {
                    player->pos.y += 1;
                }
                if (al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
                    player->pos.x -= 1;
                }
                if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
                    player->pos.x += 1;
                }
            }*/
            draw(ev);
        }
            
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
            handleMouseMove(ev.mouse.x, ev.mouse.y);
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

            // Obsługa kliknięć w przyciski
            if (ResolutionButton.hovered(ev.mouse.x, ev.mouse.y))
                changeResolution();

            else if (ResetButton.hovered(ev.mouse.x, ev.mouse.y))
                resetTimer();

            else if (CleanButton.hovered(ev.mouse.x, ev.mouse.y))
                clear();

            else if (TriangleTransformButton.hovered(ev.mouse.x, ev.mouse.y))
                TriangleTransform = !TriangleTransform;
			else if (CircleTransformButton.hovered(ev.mouse.x, ev.mouse.y))
				CircleTransform = !CircleTransform;
			else if (RectangleTransformButton.hovered(ev.mouse.x, ev.mouse.y))
				RectangleTransform = !RectangleTransform;
			else if (EllipseleTransformButton.hovered(ev.mouse.x, ev.mouse.y))
				ElipseTransform = !ElipseTransform;


            else if (RectangleButton.hovered(ev.mouse.x, ev.mouse.y))
                RectangleDrawingMode = !RectangleDrawingMode;

            else if (TriangleButton.hovered(ev.mouse.x, ev.mouse.y))
                TriangleDrawingMode = !TriangleDrawingMode;

            else if (Point2DButton.hovered(ev.mouse.x, ev.mouse.y))
                PointDrawingMode = !PointDrawingMode;

            else if (CircleCustomButton.hovered(ev.mouse.x, ev.mouse.y))
                Circle2DrawingMode = !Circle2DrawingMode;

            else if (ElipseCustomButton.hovered(ev.mouse.x, ev.mouse.y))
                ElipseDrawingMode = !ElipseDrawingMode;

            else if (PolygonButton.hovered(ev.mouse.x, ev.mouse.y))
                PolygonDrawingMode = !PolygonDrawingMode;

            else if (PolylineButton.hovered(ev.mouse.x, ev.mouse.y)) {
                PolylineDrawingMode = !PolylineDrawingMode;
                if (!PolylineDrawingMode && !polylinePoints.empty()) {
                    polylinePoints.clear();
                }
            }
            else if (Fill1Button.hovered(ev.mouse.x, ev.mouse.y))
               Fill1DrawingMode = !Fill1DrawingMode;
            else if (Fill2Button.hovered(ev.mouse.x, ev.mouse.y))
                Fill2DrawingMode = !Fill2DrawingMode;

            // Rysowanie w obszarze roboczym
            else if (PointDrawingMode) {
                Point2D newPoint(ev.mouse.x, ev.mouse.y, al_map_rgb(255, 255, 255));
                points.push_back(newPoint);
            }

            

            else if (Circle2DrawingMode) {
                Circle2Data newCircle ;
                newCircle.x0 = ev.mouse.x;
                newCircle.y0 = ev.mouse.y;
                newCircle.R = 40;
                newCircle.color = al_map_rgb(0, 255, 255);
                circles2.push_back(make_unique<CircleObject>(newCircle));
            }

            else if (ElipseDrawingMode) {
                ElipseData newElipse;
                newElipse.x0 = ev.mouse.x;
                newElipse.y0 = ev.mouse.y;
                newElipse.Rx = 40;
                newElipse.Ry = 20;
                newElipse.color = al_map_rgb(0, 0, 255);
                
                elipses.push_back(make_unique<EllipseObject>(newElipse));
                // Wyczyść tymczasowe punkty
                tempTrianglePoints.clear();
            }

            else if (PolygonDrawingMode) {
                PolygonPoint newPoly;
                newPoly.x = ev.mouse.x;
                newPoly.y = ev.mouse.y;
                punktyWielokata.push_back(newPoly);
            }

            else if (TriangleDrawingMode) {
                // Dodaj kliknięty punkt do listy tymczasowych punktów
                tempTrianglePoints.push_back({ ev.mouse.x, ev.mouse.y });

                // Jeśli mamy 3 punkty, stwórz trójkąt
                if (tempTrianglePoints.size() == 3) {
                    TriangleData newTriangle;
                    newTriangle.x0 = tempTrianglePoints[0].first;
                    newTriangle.y0 = tempTrianglePoints[0].second;
                    newTriangle.x1 = tempTrianglePoints[1].first;
                    newTriangle.y1 = tempTrianglePoints[1].second;
                    newTriangle.x2 = tempTrianglePoints[2].first;
                    newTriangle.y2 = tempTrianglePoints[2].second;
                    newTriangle.color = al_map_rgb(0, 0, 255);
                  
                    triangles.push_back(make_unique<TriangleObject>(newTriangle));
                    // Wyczyść tymczasowe punkty
                    tempTrianglePoints.clear();
                }
            }
            else if (RectangleDrawingMode) {
                // Dodaj kliknięty punkt do listy tymczasowych punktów
                tempRectanglePoints.push_back({ ev.mouse.x, ev.mouse.y });

                // Jeśli mamy 2 punkty, stwórz prostokąt
                if (tempRectanglePoints.size() == 2) {
                    RectangleData newRectangle;
                    newRectangle.x0 = tempRectanglePoints[0].first;
                    newRectangle.y0 = tempRectanglePoints[0].second;
                    newRectangle.x1 = tempRectanglePoints[1].first;
                    newRectangle.y1 = tempRectanglePoints[1].second;
                    newRectangle.color = al_map_rgb(255, 0, 255);
                    rectangles.push_back(make_unique<RectangleObject>(newRectangle));

                    // Wyczyść tymczasowe punkty
                    tempRectanglePoints.clear();
                }
            }

            
            else if (PolylineDrawingMode) {
                Point2D newPoint(ev.mouse.x, ev.mouse.y, al_map_rgb(0, 255, 0));
                polylinePoints.push_back(newPoint);
            }

            else if (Fill1DrawingMode) {
                FillPoint newPoint;
                newPoint.x = ev.mouse.x;
                newPoint.y = ev.mouse.y;
                fill1points.push_back(newPoint);
                }
            else if (Fill2DrawingMode) {
                FillPoint newPoint;
                newPoint.x = ev.mouse.x;
                newPoint.y = ev.mouse.y;
                fill2points.push_back(newPoint);
            }
        }

        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            handleKey(ev.keyboard.keycode);

            // ENTER - zakończ wielokąt
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                if (PolygonDrawingMode && !punktyWielokata.empty()) {
                    polygons.push_back(punktyWielokata);
                    punktyWielokata.clear();
                    PolygonDrawingMode = false;
                }
                else if (PolylineDrawingMode && !polylinePoints.empty()) {
                    PolylineDrawingMode = false;
                    polylinePoints.clear();
                }
            }

            // SPACE - zamknij łamaną
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE && PolylineDrawingMode) {
                if (polylinePoints.size() >= 3) {
                    PrimitiveRenderer renderer(al_map_rgb(0, 255, 0));
                    renderer.closedPolyline(polylinePoints);
                    polylinePoints.clear();
                    PolylineDrawingMode = false;
                }
            }

            if(ev.keyboard.keycode == ALLEGRO_KEY_I ) {
                if (!shapes.empty()) {
                    shapes[1]->rotate(15.0f * 3.14159265f / 180.0f, 400.0f, 300.0f); 
                }
			}
            
            if (TriangleTransform) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_I) {
                    if (!triangles.empty()) {
                        float cx, cy;
                        triangles[0]->getCenter(cx, cy);
                        triangles[0]->rotate(15.0f * 3.14159265f / 180.0f, cx, cy);
                    }
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_T) {
                    if (!triangles.empty()) {
                        triangles[0]->translate(10.0f, 10.0f);
                    }
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_S) {
                    if (!triangles.empty()) {
						float cx, cy;
						triangles[0]->getCenter(cx, cy);
                        triangles[0]->scale(1.1f, 1.1f, cx, cy);
                    }
                }
            }

            if (RectangleTransform) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_I) {
                    if (!rectangles.empty()) {
                        float cx, cy;
                        rectangles[0]->getCenter(cx, cy);
                        rectangles[0]->rotate(15.0f * 3.14159265f / 180.0f, cx, cy);
                    }
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_T) {
                    if (!rectangles.empty()) {
                        rectangles[0]->translate(10.0f, 10.0f);
                    }
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_S) {
                    if (!rectangles.empty()) {
                        float cx, cy;
                        rectangles[0]->getCenter(cx, cy);
                        rectangles[0]->scale(1.1f, 1.1f, cx, cy);
                    }
                }
            }

            if (CircleTransform) {
                if (!circles2.empty()) {
                    float cx, cy;
                    circles2[0]->getCenter(cx, cy);
                    circles2[0]->rotate(15.0f * 3.14159265f / 180.0f, cx, cy);
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_T) {
                    if (!circles2.empty()) {
                        circles2[0]->translate(10.0f, 10.0f);
                    }
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_S) {
                    if (!circles2.empty()) {
                        float cx, cy;
                        circles2[0]->getCenter(cx, cy);
                        circles2[0]->scale(1.1f, 1.1f, cx, cy);
                    }
                }
            }
            


        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            handleResize();
    }

    void handleMouseMove(int x, int y) {
        resolution_hovered = ResolutionButton.hovered(x, y);
        reset_hovered = ResetButton.hovered(x, y);
        prim_hovered = TriangleTransformButton.hovered(x, y);
    }

    void handleKey(int key) {
        switch (key) {
        case ALLEGRO_KEY_ESCAPE: running = false; break;
        case ALLEGRO_KEY_R: resetTimer(); break;
        case ALLEGRO_KEY_F: toggleFullscreen(); break;
        case ALLEGRO_KEY_DELETE:
            if (TriangleDrawingMode && !tempTrianglePoints.empty()) {
                tempTrianglePoints.clear();
            }
            if (RectangleDrawingMode && !tempRectanglePoints.empty()) {
                tempRectanglePoints.clear();
            }
            break;
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

    void changeResolution()const {
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

    // Konstruktor
    Engine() {

        initLog();
        if (!initAllegro()) exit(-1);
        createDisplay();
        loadFonts();
        createTimer();
        registerEventSources();
        setupWorkspace();
        centerWindow();
        player = make_unique<Player>(400, 300);
        logError("Silnik uruchomiony pomyslnie");
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
        logError("Silnik zakonczył dzialanie pomyslnie");
    }

    void cleanup() {
        al_destroy_timer(timer);
        if (fontSmall) al_destroy_font(fontSmall);
        if (fontLarge && fontLarge != fontSmall) al_destroy_font(fontLarge);
        if (queue) al_destroy_event_queue(queue);
        if (display) al_destroy_display(display);
    }
};
/**
 * @brief Punkt wejścia programu.
 * @return Kod wyjścia.
 */

int main() {
    Engine engine;
    engine.run();
    return 0;
}