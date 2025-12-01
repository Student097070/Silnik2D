/**
 * @file Include.h
 * @brief G³ówny plik nag³ówkowy z includes i deklaracjami zmiennych globalnych.
 * @details
 * - Zawiera wszystkie potrzebne nag³ówki Allegro i standardowe.
 * - Deklaruje zmienne globalne u¿ywane w ca³ym projekcie.
 */

#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <Windows.h>
#include <vector>
#include <map>
#include <stack>
#include <iomanip>
#include <sstream>

using namespace std;

// Komponenty biblioteki Allegro
extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_EVENT_QUEUE* queue;
extern ALLEGRO_TIMER* timer;
extern ALLEGRO_FONT* fontSmall;
extern ALLEGRO_FONT* fontLarge;
extern ALLEGRO_EVENT ev;

// ---  zmienne globalne stanu ---
extern int current_index;

extern bool running;
extern bool fullscreen;
extern bool resolution_hovered;
extern bool reset_hovered;
extern bool prim_hovered;

// Pozycje i rozmiary przycisków
extern float ResolutionButton_x, ResolutionButton_y;
extern float ResolutionButton_w, ResolutionButton_h;
extern float ResetButton_x, ResetButton_y;
extern float ResetButton_w, ResetButton_h;
extern float PrimButton_x, PrimButton_y;
extern float PrimButton_w, PrimButton_h;  
extern float WorkspacePlace_x, WorkspacePlace_y, WorkspacePlace_w, WorkspacePlace_h;
extern float Timer_x, Timer_y;
