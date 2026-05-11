#include "Algorithms.h"
#include <cmath>

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc - x, yc - y, c);

    SetPixel(hdc, xc + y, yc + x, c);
    SetPixel(hdc, xc - y, yc + x, c);
    SetPixel(hdc, xc + y, yc - x, c);
    SetPixel(hdc, xc - y, yc - x, c);
}

void LineDDA(HDC hdc, Point p1, Point p2, COLORREF c)
{
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;

    int steps = max(abs(dx), abs(dy));

    double xinc = (double)dx / steps;
    double yinc = (double)dy / steps;

    double x = p1.x;
    double y = p1.y;

    for (int i = 0; i <= steps; i++)
    {
        SetPixel(hdc, round(x), round(y), c);
        x += xinc;
        y += yinc;
    }
}

void LineMidpoint(HDC hdc, Point p1, Point p2, COLORREF c)
{
    int x1 = p1.x;
    int y1 = p1.y;

    int x2 = p2.x;
    int y2 = p2.y;

    int dx = x2 - x1;
    int dy = y2 - y1;

    int d = dx - 2 * dy;

    int x = x1;
    int y = y1;

    SetPixel(hdc, x, y, c);

    while (x < x2)
    {
        if (d <= 0)
        {
            d += 2 * dx;
        }
        else
        {
            y++;
            d += 2 * (dx - dy);
        }

        x++;
        SetPixel(hdc, x, y, c);
    }
}

void LineParametric(HDC hdc, Point p1, Point p2, COLORREF c)
{
    for (double t = 0; t <= 1; t += 0.001)
    {
        int x = p1.x + (p2.x - p1.x) * t;
        int y = p1.y + (p2.y - p1.y) * t;

        SetPixel(hdc, x, y, c);
    }
}

void CircleMidpoint(HDC hdc, Point center, int r, COLORREF c)
{
    int x = 0;
    int y = r;

    int d = 1 - r;

    Draw8Points(hdc, center.x, center.y, x, y, c);

    while (x < y)
    {
        if (d < 0)
        {
            d += 2 * x + 3;
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }

        x++;

        Draw8Points(hdc, center.x, center.y, x, y, c);
    }
}