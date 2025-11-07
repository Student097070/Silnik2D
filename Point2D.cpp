#include "Point2D.h"

Point2D::Point2D(float x, float y,ALLEGRO_COLOR kolor) {
	this->x = x;
	this->y = y;
	this->kolor = kolor;
}

void Point2D::ReadPoint(){
	if (ev.mouse.x == x && ev.mouse.y == y) {
		printf("Point coordinates: (%.2f, %.2f)\n", x, y);
	}
}	

void Point2D::ChangeCoordinates(float newX, float newY){
	x = newX;
	y = newY;
}

void Point2D::DisplayPoint(){
	al_put_pixel(x, y, kolor);
}
