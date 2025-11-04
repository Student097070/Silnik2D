#pragma once
#include "Include.h"
class Button
{
public:
	float x, y, w, h;
	ALLEGRO_COLOR ButtonCollor, TextCollor;
	string name;

	Button(float x,float y,float w, float h, string name,ALLEGRO_COLOR ButtonCollor,ALLEGRO_COLOR TextCollor);
	
	bool hovered(float mouseX,float mouseY);
	
	void draw();

	~Button();
};

