#pragma once
#include "Include.h"

class Point2D
{
public:
	float x, y; //wspolrzedne punktu 
	ALLEGRO_COLOR kolor; //kolor punktu
	Point2D(float x = 0.0, float y = 0.0);

	void ReadPoint();

	void ChangeCoordinates(float newX, float newY);

	void DisplayPoint();
	
};

