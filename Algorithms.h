#pragma once

#include <windows.h>
#include "globals.h"

// ================= HELPERS =================

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c);

//void DrawHermiteCurve(HDC hdc, Point P0, Point T0,Point P1, Point T1,int numPoints, COLORREF c);
// ================= LINES =================

void LineDDA(HDC hdc, Point p1, Point p2, COLORREF c);

void LineMidpoint(HDC hdc, Point p1, Point p2, COLORREF c);

void LineParametric(HDC hdc, Point p1, Point p2, COLORREF c);

// ================= CIRCLES =================

void CircleDirect(HDC hdc, Point center, int r, COLORREF c);

void CirclePolar(HDC hdc, Point center, int r, COLORREF c);

void CircleIterativePolar(HDC hdc, Point center, int r, COLORREF c);

void CircleMidpoint(HDC hdc, Point center, int r, COLORREF c);

void CircleModifiedMidpoint(HDC hdc, Point center, int r, COLORREF c);

// ================= ELLIPSE =================
bool PointInsideEllipse(Point p, Point center, int a, int b);

void EllipseDirect(HDC hdc, Point center, int a, int b, COLORREF c);

void EllipsePolar(HDC hdc, Point center, int a, int b, COLORREF c);

void EllipseMidpoint(HDC hdc, Point center, int a, int b, COLORREF c);

// ================= CURVES =================

void DrawCardinalSpline(HDC hdc, const vector<Point> &pts, float tension, COLORREF c);


// ================== FILLING ===============
void FillCircleWithLines(HDC hdc, Point c, int r, int quarter, COLORREF color);

void FillSquareWithHermite(HDC hdc,
                           int x1,
                           int y1,
                           int side,
                           COLORREF c);

bool PointInsideCircle(Point p, Point center, int r);

void FloodFillRec(HDC hdc,
                  int x,
                  int y,
                  COLORREF oldColor,
                  COLORREF fillColor);

void FillCircleWithCircles(HDC hdc, Point center, int r, Point quarter, COLORREF c);

void FillRectangleBezier(HDC hdc, Point p1, Point p2, COLORREF c);

void ConvexFill(HDC hdc, vector<Point> polygon, COLORREF c);

void NonConvexFill(HDC hdc, vector<Point> polygon, COLORREF c);


void NonRecursiveFloodFill(HDC hdc, int x, int y, COLORREF fillColor, COLORREF boundaryColor);

// ================= CLIPPING =================

void RectanglePointClipping(HDC hdc, int x, int y, int xleft, int xright, int ybottom, int ytop, COLORREF color);

void RectangleLineClipping(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int xright, int ybottom, int ytop,
                           COLORREF color);

void RectanglePolygonClipping(HDC hdc, Point *p, int n, int xleft, int ytop, int xright, int ybottom, COLORREF color);

void SquarePointClipping(HDC hdc, int x, int y, int xleft, int xright, int ybottom, int ytop, COLORREF color);

void SquareLineClipping(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int xright, int ybottom, int ytop,
                        COLORREF color);

// ================= BONUS =================

void CirclePointClipping(HDC hdc, int x, int y, int cx, int cy, int r, COLORREF color);

void CircleLineClipping(HDC hdc, int xs, int ys, int xe, int ye, int cx, int cy, int r, COLORREF color);

void DrawFace(HDC hdc, Point center, int r, MouthType type, COLORREF c);
