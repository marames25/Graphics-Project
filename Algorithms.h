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

void EllipseDirect(HDC hdc, Point center, int a, int b, COLORREF c);

void EllipsePolar(HDC hdc, Point center, int a, int b, COLORREF c);

void EllipseMidpoint(HDC hdc, Point center, int a, int b, COLORREF c);

// ================= CURVES =================

void DrawCardinalSpline(HDC hdc, const vector<Point>& pts, float tension, COLORREF c);
// ================= FILLING =================

void FillCircleWithLines(HDC hdc, Point center, int r, int quarter, COLORREF c);

void FillCircleWithCircles(HDC hdc, Point center, int r, int quarter, COLORREF c);

void FillSquareHermit(HDC hdc, Point p1, Point p2, COLORREF c);

void FillRectangleBezier(HDC hdc, Point p1, Point p2, COLORREF c);

void ConvexFill(HDC hdc, vector<Point> polygon, COLORREF c);

void NonConvexFill(HDC hdc, vector<Point> polygon, COLORREF c);

void RecursiveFloodFill(HDC hdc, int x, int y, COLORREF fillColor, COLORREF boundaryColor);

void NonRecursiveFloodFill(HDC hdc, int x, int y, COLORREF fillColor, COLORREF boundaryColor);

// ================= CLIPPING =================

void RectanglePointClipping(HDC hdc, Point p, Point p1, Point p2);

void RectangleLineClipping(HDC hdc, Point p1, Point p2, Point clip1, Point clip2);

void RectanglePolygonClipping(HDC hdc, vector<Point> polygon, Point clip1, Point clip2);

void SquarePointClipping(HDC hdc, Point p, Point p1, Point p2, COLORREF c);

void SquareLineClipping(HDC hdc, Point p1, Point p2, Point clip1, Point clip2, COLORREF c);

// ================= BONUS =================

void CirclePointClipping(HDC hdc, Point p, Point center, int r);

void CircleLineClipping(HDC hdc, Point p1, Point p2, Point center, int r);

void DrawFace(HDC hdc, Point center, int r, MouthType type, COLORREF c);