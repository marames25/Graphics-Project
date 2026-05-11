#include <windows.h>
#include <bits/stdc++.h>
#include "globals.h"
#include "Algorithms.h"
using namespace std;



Mode CurrentMode = NONE;
COLORREF CurrentColor = RGB(255,0,0);

vector<Point> TempPoints;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#define IDM_DDA 1
#define IDM_MIDPOINT 2
#define IDM_PARAMETRIC 3
#define IDM_CIRCLE 4

void AddMenus(HWND hwnd)
{
    HMENU hMenu = CreateMenu();

    HMENU hLines = CreateMenu();
    HMENU hCircles = CreateMenu();

    AppendMenu(hLines, MF_STRING, IDM_DDA, "DDA");
    AppendMenu(hLines, MF_STRING, IDM_MIDPOINT, "Midpoint");
    AppendMenu(hLines, MF_STRING, IDM_PARAMETRIC, "Parametric");

    AppendMenu(hCircles, MF_STRING, IDM_CIRCLE, "Circle Midpoint");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hLines, "Lines");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hCircles, "Circles");

    SetMenu(hwnd, hMenu);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASS wc = {};

    wc.lpszClassName = "Graphics";
    wc.hInstance = hInst;
    wc.lpfnWndProc = WindowProcedure;
    wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
            "Graphics",
            "2D Drawing Package",
            WS_OVERLAPPEDWINDOW,
            100,
            100,
            1000,
            700,
            NULL,
            NULL,
            hInst,
            NULL
    );

    AddMenus(hwnd);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    HDC hdc = GetDC(hwnd);

    switch(msg)
    {
        case WM_COMMAND:

            switch(wp)
            {
                case IDM_DDA:
                    CurrentMode = LINE_DDA;
                    break;

                case IDM_MIDPOINT:
                    CurrentMode = LINE_MIDPOINT;
                    break;

                case IDM_PARAMETRIC:
                    CurrentMode = LINE_PARAMETRIC;
                    break;

                case IDM_CIRCLE:
                    CurrentMode = CIRCLE_MIDPOINT;
                    break;
            }

            break;

        case WM_LBUTTONDOWN:

            TempPoints.push_back(Point(LOWORD(lp), HIWORD(lp)));

            if (TempPoints.size() == 2)
            {
                Point p1 = TempPoints[0];
                Point p2 = TempPoints[1];

                switch(CurrentMode)
                {
                    case LINE_DDA:
                        LineDDA(hdc, p1, p2, CurrentColor);
                        break;

                    case LINE_MIDPOINT:
                        LineMidpoint(hdc, p1, p2, CurrentColor);
                        break;

                    case LINE_PARAMETRIC:
                        LineParametric(hdc, p1, p2, CurrentColor);
                        break;

                    case CIRCLE_MIDPOINT:
                    {
                        int dx = p2.x - p1.x;
                        int dy = p2.y - p1.y;

                        int r = sqrt(dx*dx + dy*dy);

                        CircleMidpoint(hdc, p1, r, CurrentColor);
                        break;
                    }
                }

                TempPoints.clear();
            }

            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    ReleaseDC(hwnd, hdc);

    return DefWindowProc(hwnd, msg, wp, lp);
}
