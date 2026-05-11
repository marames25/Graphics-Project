#pragma once
#include <windows.h>
#include <vector>
using namespace std;

struct Point
{
    int x, y;
    Point() {}
    Point(int a, int b)
    {
        x = a;
        y = b;
    }
};

enum Mode
{
    NONE,

    // Lines
    LINE_DDA,
    LINE_MIDPOINT,
    LINE_PARAMETRIC,

    // Circles
    CIRCLE_DIRECT,
    CIRCLE_POLAR,
    CIRCLE_ITERATIVE,
    CIRCLE_MIDPOINT,
    CIRCLE_MODIFIED,

    // Ellipse
    ELLIPSE_DIRECT,
    ELLIPSE_POLAR,
    ELLIPSE_MIDPOINT,

    // Curves
    CARDINAL_SPLINE,

    // Filling
    FILL_CIRCLE_LINES,
    FILL_CIRCLE_CIRCLES,
    FILL_SQUARE_HERMIT,
    FILL_RECT_BEZIER,
    FLOOD_FILL_RECURSIVE,
    FLOOD_FILL_NON_RECURSIVE,

    // Clipping
    CLIP_RECT_POINT,
    CLIP_RECT_LINE,
    CLIP_RECT_POLYGON,

    CLIP_SQUARE_POINT,
    CLIP_SQUARE_LINE
};

extern Mode CurrentMode;
extern COLORREF CurrentColor;

extern vector<Point> TempPoints;