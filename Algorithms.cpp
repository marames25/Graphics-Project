#include <windows.h>
#include <bits/stdc++.h>
#include "Algorithms.h"
//============== HELPERS
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
//void DrawHermiteCurve(HDC hdc, Point P0, Point T0,
//                      Point P1, Point T1,
//                      int n, COLORREF c)
//{
//    if (n < 2) return;
//
//    double dt = 1.0 / (n - 1);
//
//    double x_prev, y_prev;
//
//    for (double t = 0; t <= 1; t += dt)
//    {
//        double h1 = 2*t*t*t - 3*t*t + 1;
//        double h2 = t*t*t - 2*t*t + t;
//        double h3 = -2*t*t*t + 3*t*t;
//        double h4 = t*t*t - t*t;
//
//        double x = h1*P0.x + h2*T0.x + h3*P1.x + h4*T1.x;
//        double y = h1*P0.y + h2*T0.y + h3*P1.y + h4*T1.y;
//
//        if (t == 0)
//        {
//            MoveToEx(hdc, x, y, NULL);
//        }
//        else
//        {
//            LineTo(hdc, x, y);
//        }
//    }
//}
//============== LINE
void LineDDA(HDC hdc, Point p1, Point p2, COLORREF c)
{
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;

    int steps = max(abs(dx), abs(dy));

    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = p1.x;
    float y = p1.y;

    for (int i = 0; i <= steps; i++)
    {
        SetPixel(hdc, round(x), round(y), c);
        x += xInc;
        y += yInc;
    }
}

void LineMidpoint(HDC hdc, Point p1, Point p2, COLORREF color)
{
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;

    int sx = (dx >= 0) ? 1 : -1;
    int sy = (dy >= 0) ? 1 : -1;

    dx = abs(dx);
    dy = abs(dy);

    int x = p1.x;
    int y = p1.y;

    SetPixel(hdc, x, y, color);

    // case 1: slope <= 1 (move in x)
    if (dx >= dy)
    {
        int d = 2 * dy - dx;
        int dE = 2 * dy;
        int dNE = 2 * (dy - dx);

        for (int i = 0; i < dx; i++)
        {
            if (d <= 0)
            {
                d += dE;
                x += sx;
            }
            else
            {
                d += dNE;
                x += sx;
                y += sy;
            }

            SetPixel(hdc, x, y, color);
        }
    }
        // case 2: slope > 1 (move in y)
    else
    {
        int d = 2 * dx - dy;
        int dN = 2 * dx;
        int dNE = 2 * (dx - dy);

        for (int i = 0; i < dy; i++)
        {
            if (d <= 0)
            {
                d += dN;
                y += sy;
            }
            else
            {
                d += dNE;
                x += sx;
                y += sy;
            }

            SetPixel(hdc, x, y, color);
        }
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
//============== CIRCLE

void CircleDirect(HDC hdc, Point center, int r, COLORREF c)
{
    int xc = center.x;
    int yc = center.y;

    int x = 0;
    int R2 = r * r;

    int y = r;

    Draw8Points(hdc, xc, yc, x, y, c);

    while (x < y)
    {
        x++;

        y = (int)round(sqrt(R2 - x * x));

        Draw8Points(hdc, xc, yc, x, y, c);
    }
}

void CirclePolar(HDC hdc, Point center, int r, COLORREF c)
{
    double theta = 0;
    double dtheta = 1.0 / r;

    Point p;
    p.x = r;
    p.y = 0;

    Draw8Points(hdc, center.x, center.y, p.x, p.y, c);

    while (p.x >= p.y)
    {
        theta += dtheta;

        p.x = round(r * cos(theta));
        p.y = round(r * sin(theta));

        Draw8Points(hdc, center.x, center.y, p.x, p.y, c);
    }
}

void CircleIterativePolar(HDC hdc, Point center, int r, COLORREF c)
{
    double x = r;
    double y = 0;

    double dtheta = 1.0 / r;

    double cdtheta = cos(dtheta);
    double sdtheta = sin(dtheta);

    Draw8Points(hdc, center.x, center.y, (int)round(x), (int)round(y), c);

    while (x >= y)
    {
        double x1 = x * cdtheta - y * sdtheta;
        y = x * sdtheta + y * cdtheta;
        x = x1;

        Draw8Points(hdc, center.x, center.y,
                    (int)round(x),
                    (int)round(y),
                    c);
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

void CircleModifiedMidpoint(HDC hdc, Point center, int r, COLORREF c)
{
    int x = 0;
    int y = r;

    int d = 1 - r;

    int dE = 3;
    int dSE = 5 - 2 * r;

    Draw8Points(hdc, center.x, center.y, x, y, c);

    while (x < y)
    {
        if (d < 0)
        {
            d += dE;
            dSE += 2;
        }
        else
        {
            d += dSE;
            dSE += 4;
            y--;
        }

        dE += 2;
        x++;

        Draw8Points(hdc, center.x, center.y, x, y, c);
    }
}

//============== CURVE
void DrawCardinalSpline(HDC hdc, const vector<Point>& pts, float tension, COLORREF c)
{
    if (pts.size() < 2) return;

    int n = (int)pts.size();
    int steps = 100;

    for (int i = 0; i < n - 1; ++i)
    {
        Point p0 = pts[max(0, i - 1)];
        Point p1 = pts[i];
        Point p2 = pts[i + 1];
        Point p3 = pts[min(n - 1, i + 2)];

        bool first = true;

        for (int s = 0; s <= steps; ++s)
        {
            float t  = (float)s / steps;
            float t2 = t * t;
            float t3 = t2 * t;

            float h1 =  2*t3 - 3*t2 + 1;
            float h2 = -2*t3 + 3*t2;
            float h3 =     t3 - 2*t2 + t;
            float h4 =     t3 -   t2;

            float x =
                    h1 * p1.x +
                    h2 * p2.x +
                    tension * h3 * (p2.x - p0.x) +
                    tension * h4 * (p3.x - p1.x);

            float y =
                    h1 * p1.y +
                    h2 * p2.y +
                    tension * h3 * (p2.y - p0.y) +
                    tension * h4 * (p3.y - p1.y);

            if (first)
            {
                MoveToEx(hdc, (int)x, (int)y, NULL);
                first = false;
            }
            else
            {
                LineTo(hdc, (int)x, (int)y);
            }
        }
    }
}


//============== HAPPY /SAD FACE
void DrawFace(HDC hdc, Point center, int r, MouthType type, COLORREF c)
{
    // face
    CircleMidpoint(hdc, center, r, c);

    // eye
    CircleMidpoint(hdc, Point(center.x - r/3, center.y - r/3), r/10, RGB(0,0,0));
    CircleMidpoint(hdc, Point(center.x + r/3, center.y - r/3), r/10, RGB(0,0,0));

    // nose
    LineMidpoint(
            hdc,
            Point(center.x, center.y - r/6),
            Point(center.x, center.y + r/6),
            RGB(0,0,0)
    );

    // mouth
    for (double t = -1; t <= 1; t += 0.01)
    {
        int x = center.x + t * r/2;

        int y;
        if (type == HAPPY)
            y = center.y + r/3 + (t * t) * r/3;   // happy
        else
            y = center.y + r/2 - (t * t) * r/3;   // sad

        SetPixel(hdc, x, y, RGB(0,0,0));
    }
}


void NonRecursiveFloodFill(HDC hdc, int x, int y, COLORREF fillColor, COLORREF boundaryColor) {
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    queue <Point> q;
    q.push({x,y});
    
    while (!q.empty()) {
        Point pnt = q.front();
        q.pop();
        
        COLORREF c = GetPixel(hdc, pnt.x, pnt.y);
        if (c == boundaryColor || c == fillColor) continue;
        
        SetPixel(hdc, pnt.x, pnt.y, fillColor);
        for (int i = 0; i < 4; i++) {
            int Nx = pnt.x+dx[i], Ny = pnt.y + dy[i];
            q.push({Nx, Ny});
        }
    }
}