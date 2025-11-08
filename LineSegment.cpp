#include "LineSegment.h"

//LineSegment::LineSegment(int x0, int y0, int x1, int y1){
//	this->x0 = x0;
//	this->y0 = y0;
//	this->x1 = x1;
//	this->y1 = y1;
//}

void LineSegment::DrawLine(Point2D p1,Point2D p2){
	line(p1.x, p1.y, p2.x, p2.y);
}

LineSegment::~LineSegment(){}
