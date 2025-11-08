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

struct LineData {
	int x0, y0, x1, y1;
	ALLEGRO_COLOR color;
};

