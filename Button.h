/**
 * @file Button.h
 * @brief Nag³ówek klasy Button – przycisku interfejsu u¿ytkownika.
 * @details
 * - Obs³uguje stan najechania myszk¹ (hover).
 * - Rysuje prostok¹t z tekstem.
 */
#pragma once
#include "Include.h"
 /**
  * @class Button
  * @brief Klasa reprezentuj¹ca przycisk UI.
  */
class Button
{
public:
	float x, y, w, h;  ///< Pozycja i rozmiary
	ALLEGRO_COLOR ButtonCollor, TextCollor; ///< Kolory przycisku i tekstu
	string name; ///< Tekst przycisku

	Button(float x,float y,float w, float h, string name,ALLEGRO_COLOR ButtonCollor,ALLEGRO_COLOR TextCollor);

	bool hovered(float mouseX, float mouseY); // funkcja sprawdzajaca czy myszka jest nad przyciskiem
	
	void draw(); //funckja rysujaca przycisk

	~Button();
};

