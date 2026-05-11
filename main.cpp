#include <windows.h>
#include <vector>
#include <cmath>

#include "globals.h"
#include "Algorithms.h"

using namespace std;

// ================= GLOBAL DEFINITIONS =================
Mode CurrentMode = NONE;
COLORREF CurrentColor = RGB(255, 0, 0);
vector<Point> TempPoints;

// ================= HELPERS =================
void ClearScreen(HWND hwnd)
{
    InvalidateRect(hwnd, NULL, TRUE);
}

// ================= MENU IDS =================
#define IDM_CLEAR 1

// Lines
#define IDM_DDA 10
#define IDM_MIDPOINT 11
#define IDM_PARAMETRIC 12

// Circles
#define IDM_CIRCLE_MIDPOINT 20

// Faces
#define IDM_HAPPY 30
#define IDM_SAD 31

// ================= MENU CREATION =================
void AddMenus(HWND hwnd)
{
    HMENU menu = CreateMenu();

    HMENU hLines = CreateMenu();
    AppendMenu(hLines, MF_STRING, IDM_DDA, "DDA");
    AppendMenu(hLines, MF_STRING, IDM_MIDPOINT, "Midpoint");
    AppendMenu(hLines, MF_STRING, IDM_PARAMETRIC, "Parametric");

    HMENU hCircle = CreateMenu();
    AppendMenu(hCircle, MF_STRING, IDM_CIRCLE_MIDPOINT, "Midpoint Circle");

    HMENU hFace = CreateMenu();
    AppendMenu(hFace, MF_STRING, IDM_HAPPY, "Happy Face");
    AppendMenu(hFace, MF_STRING, IDM_SAD, "Sad Face");

    AppendMenu(menu, MF_POPUP, (UINT_PTR)hLines, "Lines");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)hCircle, "Circles");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)hFace, "Faces");

    AppendMenu(menu, MF_STRING, IDM_CLEAR, "Clear Screen");

    SetMenu(hwnd, menu);
}

// ================= WINDOW PROCEDURE =================
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    HDC hdc = GetDC(hwnd);

    switch(msg)
    {
        // ================= MENU =================
        case WM_COMMAND:
        {
            switch(LOWORD(wp))
            {
                case IDM_CLEAR:
                    ClearScreen(hwnd);
                    break;

                case IDM_DDA:
                    CurrentMode = LINE_DDA;
                    break;

                case IDM_MIDPOINT:
                    CurrentMode = LINE_MIDPOINT;
                    break;

                case IDM_PARAMETRIC:
                    CurrentMode = LINE_PARAMETRIC;
                    break;

                case IDM_CIRCLE_MIDPOINT:
                    CurrentMode = CIRCLE_MIDPOINT;
                    break;

                case IDM_HAPPY:
                    CurrentMode = HAPPY_FACE;
                    break;

                case IDM_SAD:
                    CurrentMode = SAD_FACE;
                    break;
            }
            break;
        }

            // ================= MOUSE DRAW =================
        case WM_LBUTTONDOWN:
        {
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
                        int r = (int)sqrt(dx*dx + dy*dy);

                        CircleMidpoint(hdc, p1, r, CurrentColor);
                        break;
                    }

                    case HAPPY_FACE:
                        DrawHappyFace(hdc, p1, 50);
                        break;

                    case SAD_FACE:
                        DrawSadFace(hdc, p1, 50);
                        break;
                }

                TempPoints.clear();
            }
            break;
        }

            // ================= EXIT =================
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    ReleaseDC(hwnd, hdc);
    return DefWindowProc(hwnd, msg, wp, lp);
}

// ================= WINMAIN =================
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInst;
    wc.lpszClassName = "DrawingApp";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
            "DrawingApp",
            "2D Drawing Package",
            WS_OVERLAPPEDWINDOW,
            100, 100, 900, 600,
            NULL, NULL, hInst, NULL
    );

    AddMenus(hwnd);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}