/**
 * @file STR.h
 * @brief Nag³ówek z strukturami danych u¿ywanych w programie.
 * @details
 * - Zawiera definicje struktur dla rozdzielczoœci, kszta³tów geometrycznych i punktów.
 */
#pragma once

 /**
  * @struct Resolution
  * @brief Struktura przechowuj¹ca informacje o rozdzielczoœci okna i obszaru roboczego.
  */

struct Resolution {
    int w, h;                           // Szerokoœæ i wysokoœæ okna
    int workspace_w, workspace_h;       // Szerokoœæ i wysokoœæ obszaru roboczego
};

/**
 * @struct CircleData
 * @brief Dane okrêgu (starsza wersja).
 */

struct CircleData {
    int x, y, r;
    ALLEGRO_COLOR color;
};
/**
 * @struct TriangleData
 * @brief Dane trójk¹ta.
 */
struct TriangleData {
	float x0, y0, x1, y1, x2, y2;
    ALLEGRO_COLOR color;
};
/**
 * @struct RectangleData
 * @brief Dane prostok¹ta.
 */
struct RectangleData {
	float x0, y0, x1, y1;
	ALLEGRO_COLOR color;
};
/**
 * @struct Circle2Data
 * @brief Dane okrêgu (nowsza wersja).
 */
struct Circle2Data {
	float x0, y0, R;
	ALLEGRO_COLOR color;
};
/**
 * @struct ElipseData
 * @brief Dane elipsy.
 */
struct ElipseData {
	float x0, y0, Rx, Ry;
	ALLEGRO_COLOR color;
};
/**
 * @struct PolygonPoint
 * @brief Punkt wielok¹ta.
 */
struct PolygonPoint {
	float x, y;
};
/**
 * @struct PolygonData
 * @brief Dane wielok¹ta.
 */
struct PolygonData {
	vector<PolygonPoint>& points;
	ALLEGRO_COLOR color;
};
/**
 * @struct FillPoint
 * @brief Punkt wype³nienia.
 */
struct FillPoint {
	float x, y;
};