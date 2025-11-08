#pragma once
#include "Include.h"
#include "PrimitiveRenderer.h"
#include "Point2D.h"

class LineSegment : public PrimitiveRenderer{
public:
	/*int x0, y0, x1, y1;
	LineSegment(int x0, int y0, int x1, int y1);*/

	void DrawLine(Point2D p1, Point2D p2);

	~LineSegment();
};

