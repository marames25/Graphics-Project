#pragma once
#include <windows.h>
#include "globals.h"

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c);

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