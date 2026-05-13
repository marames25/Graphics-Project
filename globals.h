#pragma once
#include <windows.h>
#include <vector>

using namespace std;

// ================= POINT =================
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
enum MouthType
{
    HAPPY = 0,
    SAD = 1
};
enum FillCircleStep
{
    WAIT_CENTER,
    WAIT_RADIUS,
    WAIT_QUARTER
};
struct CircleData {
    Point center;
    int radius;
};
struct EllipseData {
    Point center;
    int a, b; // semi-axes
};


// ================= DRAW MODES =================
enum Mode
{
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
    FILL_SQUARE_HERMITE,
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
    SAD_FACE
};

// ================= GLOBAL VARIABLES =================
extern Mode CurrentMode;

extern COLORREF CurrentColor;

extern vector<Point> TempPoints;
extern FillCircleStep step;
extern Point center;
extern Point radiusPoint;
extern Point quarterPoint;

extern COLORREF BoundaryColor;
extern COLORREF BackgroundColor;

extern vector<CircleData> DrawnCircles;
extern vector<EllipseData> DrawnEllipses;