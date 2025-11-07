#pragma once
#include "Include.h"

class Point2D
{
public:
	float x, y; //wspolrzedne punktu 
	ALLEGRO_COLOR kolor; //kolor punktu
	Point2D(float x, float y, ALLEGRO_COLOR kolor);

	void ReadPoint();

	void ChangeCoordinates(float newX, float newY);

	void DisplayPoint();
	
};

