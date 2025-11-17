#include "Include.h"

ALLEGRO_DISPLAY* display = nullptr;
ALLEGRO_EVENT_QUEUE* queue = nullptr;
ALLEGRO_TIMER* timer = nullptr;
ALLEGRO_FONT* fontSmall = nullptr;
ALLEGRO_FONT* fontLarge = nullptr;
ALLEGRO_EVENT ev;

// --- Aplikacja: zmienne globalne stanu ---
int current_index = 0;

bool running = true;
bool fullscreen = false;
bool resolution_hovered = false;
bool reset_hovered = false;
bool prim_hovered = false;

// --- UI: pozycje i rozmiary przycisków ---

float ResetButton_x = 110, ResetButton_y = 0;
float ResetButton_w = 100, ResetButton_h = 20;
float PrimButton_x = 220, PrimButton_y = 0;
float PrimButton_w = 100, PrimButton_h = 20; 
float WorkspacePlace_x, WorkspacePlace_y, WorkspacePlace_w, WorkspacePlace_h;
float Timer_x, Timer_y;
