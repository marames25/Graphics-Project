#pragma once
#include <windows.h>
#include <vector>

using namespace std;

// ================= POINT =================
struct Point {
	int x, y;

	Point() {
	}

	Point(int a, int b) {
		x = a;
		y = b;
	}
};

enum MouthType {
	HAPPY = 0,
	SAD = 1
};

union OutCode {
	struct {
		unsigned L: 1, R: 1, B: 1, T: 1;
	};

	unsigned all: 4;
};

struct Vertex {
	double x, y;

	Vertex(double x = 0, double y = 0) {
		this->x = x;
		this->y = y;
	}
};

typedef vector<Vertex> VertexList;

typedef bool (*IsInFunc)(Vertex &v, int edge);

typedef Vertex (*IntersectFunc)(Vertex &v1, Vertex &v2, int edge);

struct Line {
	int x1, y1;
	int x2, y2;
	COLORREF color;

	Line(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, COLORREF color = RGB(0, 0, 0)) {
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
		this->color = color;
	}
};

// ================= DRAW MODES =================
enum Mode {
	NONE,

	// ================= LINES =================
	LINE_DDA,
	LINE_MIDPOINT,
	LINE_PARAMETRIC,

	// ================= CIRCLES =================
	CIRCLE_DIRECT,
	CIRCLE_POLAR,
	CIRCLE_ITERATIVE_POLAR,
	CIRCLE_MIDPOINT,
	CIRCLE_MODIFIED,

	// ================= ELLIPSE =================
	ELLIPSE_DIRECT,
	ELLIPSE_POLAR,
	ELLIPSE_MIDPOINT,

	// ================= CURVES =================
	CARDINAL_SPLINE,

	// ================= FILLING =================
	FILL_CIRCLE_LINES,
	FILL_CIRCLE_CIRCLES,
	FILL_SQUARE_HERMIT,
	FILL_RECT_BEZIER,
	CONVEX_FILL,
	NON_CONVEX_FILL,
	FLOOD_FILL_RECURSIVE,
	FLOOD_FILL_NON_RECURSIVE,

	// ================= CLIPPING =================
	CLIP_RECT_POINT,
	CLIP_RECT_LINE,
	CLIP_RECT_POLYGON,

	CLIP_SQUARE_POINT,
	CLIP_SQUARE_LINE,

	// ================= BONUS =================
	CLIP_CIRCLE_POINT,
	CLIP_CIRCLE_LINE,

	HAPPY_FACE,
	SAD_FACE,

	// ================= SHAPES =================
	PENTAGON
};

// ================= GLOBAL VARIABLES =================
extern Mode CurrentMode;

extern COLORREF CurrentColor;

extern vector<Point> TempPoints;

extern vector<Point> Points;

extern vector<Line> Lines;

extern vector<VertexList> Polygons;
