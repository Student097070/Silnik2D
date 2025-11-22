#pragma once
// Struktura przechowuj¹ca informacje o rozdzielczoœci okna i obszaru roboczego
struct Resolution {
    int w, h;                           // Szerokoœæ i wysokoœæ okna
    int workspace_w, workspace_h;       // Szerokoœæ i wysokoœæ obszaru roboczego
};

struct CircleData {
    int x, y, r;
    ALLEGRO_COLOR color;
};

struct TriangleData {
	int x0, y0, x1, y1, x2, y2;
    ALLEGRO_COLOR color;
};

struct RectangleData {
	int x0, y0, x1, y1;
	ALLEGRO_COLOR color;
};

struct Circle2Data {
	int x0, y0, R;
	ALLEGRO_COLOR color;
};

struct ElipseData {
	int x0, y0, Rx, Ry;
	ALLEGRO_COLOR color;
};

struct PolygonPoint {
	float x, y;
};

struct PolygonData {
	vector<PolygonPoint>& points;
	ALLEGRO_COLOR color;
};

struct FillPoint {
	float x, y;
};