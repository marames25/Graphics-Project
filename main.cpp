#include <windows.h>
#include <vector>
#include <cmath>
#include <string>

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
#define IDM_SAVE 2
#define IDM_LOAD 3

// Lines
#define IDM_DDA 10
#define IDM_MIDPOINT 11
#define IDM_PARAMETRIC 12

// Circles
#define IDM_CIRCLE_DIRECT 20
#define IDM_CIRCLE_POLAR 21
#define IDM_CIRCLE_ITERATIVE 22
#define IDM_CIRCLE_MIDPOINT 23
#define IDM_CIRCLE_MODIFIED 24

// Ellipse
#define IDM_ELLIPSE_DIRECT   50
#define IDM_ELLIPSE_POLAR    51
#define IDM_ELLIPSE_MIDPOINT 52

// Curves
#define IDM_CARDINAL 40

// Faces
#define IDM_HAPPY 30
#define IDM_SAD 31


// Preferences
#define IDM_BG_WHITE      80
#define IDM_CURSOR_CROSS  81
#define IDM_CURSOR_HAND   82
#define IDM_COLOR_RED     83
#define IDM_COLOR_GREEN   84
#define IDM_COLOR_BLUE    85
#define IDM_COLOR_BLACK   86
#define IDM_COLOR_YELLOW  87

// Clipping
#define IDM_CLIP_SQ_POINT 90
#define IDM_CLIP_SQ_LINE  91

// Filling
#define IDM_FILL_RECT_BEZIER  71
#define IDM_FILL_CIRCLE_LINES 60
#define IDM_FILL_CIRCLE_CIRCLES 61
#define IDM_FILL_SQUARE_HERMITE 62
#define IDM_CONVEX_FILL 64
#define IDM_NON_CONVEX_FILL 65
#define IDM_FLOOD_FILL_RECURSIVE 66
#define IDM_FLOOD_FILL_NON_RECURSIVE 67
// ================= MENU CREATION =================
void AddMenus(HWND hwnd)
{
    HMENU menu = CreateMenu();

    HMENU hFile = CreateMenu();
    AppendMenu(hFile, MF_STRING, IDM_SAVE, "Save");
    AppendMenu(hFile, MF_STRING, IDM_LOAD, "Load");
    AppendMenu(hFile, MF_STRING, IDM_CLEAR, "Clear");

    HMENU hLines = CreateMenu();
    AppendMenu(hLines, MF_STRING, IDM_DDA, "DDA");
    AppendMenu(hLines, MF_STRING, IDM_MIDPOINT, "Midpoint");
    AppendMenu(hLines, MF_STRING, IDM_PARAMETRIC, "Parametric");

    HMENU hCircle = CreateMenu();
    AppendMenu(hCircle, MF_STRING, IDM_CIRCLE_DIRECT, "Direct Circle");
    AppendMenu(hCircle, MF_STRING, IDM_CIRCLE_POLAR, "Polar Circle");
    AppendMenu(hCircle, MF_STRING, IDM_CIRCLE_ITERATIVE, "Iterative Polar");
    AppendMenu(hCircle, MF_STRING, IDM_CIRCLE_MIDPOINT, "Midpoint Circle");
    AppendMenu(hCircle, MF_STRING, IDM_CIRCLE_MODIFIED, "Modified Midpoint");



    HMENU hCurves = CreateMenu();
    AppendMenu(hCurves, MF_STRING, IDM_CARDINAL, "Cardinal Spline");


    HMENU hFace = CreateMenu();
    AppendMenu(hFace, MF_STRING, IDM_HAPPY, "Sad Face");
    AppendMenu(hFace, MF_STRING, IDM_SAD, "Happy Face");

    HMENU hPrefs = CreateMenu();
    AppendMenu(hPrefs, MF_STRING, IDM_BG_WHITE,     "White Background");
    AppendMenu(hPrefs, MF_SEPARATOR, 0, NULL);
    AppendMenu(hPrefs, MF_STRING, IDM_CURSOR_CROSS, "Cursor: Crosshair");
    AppendMenu(hPrefs, MF_STRING, IDM_CURSOR_HAND,  "Cursor: Hand");
    AppendMenu(hPrefs, MF_SEPARATOR, 0, NULL);
    AppendMenu(hPrefs, MF_STRING, IDM_COLOR_RED,    "Color: Red");
    AppendMenu(hPrefs, MF_STRING, IDM_COLOR_GREEN,  "Color: Green");
    AppendMenu(hPrefs, MF_STRING, IDM_COLOR_BLUE,   "Color: Blue");
    AppendMenu(hPrefs, MF_STRING, IDM_COLOR_BLACK,  "Color: Black");
    AppendMenu(hPrefs, MF_STRING, IDM_COLOR_YELLOW, "Color: Yellow");


    AppendMenu(menu, MF_POPUP, (UINT_PTR)hFile, "File");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)hPrefs, "Preferences");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)hLines, "Lines");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)hCircle, "Circles");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)hFace, "Faces");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)hCurves, "Curves");

    HMENU hFill = CreateMenu();
    AppendMenu(hFill, MF_STRING, IDM_FILL_CIRCLE_LINES, "Fill Circle (Lines)");
    AppendMenu(hFill, MF_STRING, IDM_FILL_CIRCLE_CIRCLES, "Fill Circle (Circles)");
    AppendMenu(hFill, MF_STRING, IDM_FILL_SQUARE_HERMITE, "Fill Square (Hermite)");
    AppendMenu(hFill, MF_STRING, IDM_FLOOD_FILL_RECURSIVE, "Flood Fill Recursive");
    AppendMenu(hFill, MF_STRING, IDM_FLOOD_FILL_NON_RECURSIVE, "Flood Fill Non Recursive");
    AppendMenu(hFill, MF_STRING, IDM_FILL_RECT_BEZIER, "Fill Rect Bezier (H)");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)hFill, "Filling");
    SetMenu(hwnd, menu);

    HMENU hEllipse = CreateMenu();
    AppendMenu(hEllipse, MF_STRING, IDM_ELLIPSE_DIRECT,   "Direct Ellipse");
    AppendMenu(hEllipse, MF_STRING, IDM_ELLIPSE_POLAR,    "Polar Ellipse");
    AppendMenu(hEllipse, MF_STRING, IDM_ELLIPSE_MIDPOINT, "Midpoint Ellipse");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)hEllipse, "Ellipses");
}

// ================= Save & Load BMP Images =================
void SaveBMP(HWND hwnd, const char* filename)
{
    // Get window DC
    HDC hdcWindow = GetDC(hwnd);

    // Get window size
    RECT rect;
    GetClientRect(hwnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Create memory DC
    HDC hdcMem = CreateCompatibleDC(hdcWindow);

    // Create bitmap
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcWindow, width, height);

    // Select bitmap into memory DC
    SelectObject(hdcMem, hBitmap);

    // Copy window content to memory bitmap
    BitBlt(hdcMem, 0, 0, width, height,
           hdcWindow, 0, 0, SRCCOPY);

    // Bitmap info
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    BITMAPINFOHEADER bi;
    ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = bmp.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;

    // Calculate bitmap size
    DWORD bmpSize =
        ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;

    // Allocate memory for pixels
    char* pixels = new char[bmpSize];

    // Get bitmap pixels
    GetDIBits(hdcWindow,
              hBitmap,
              0,
              bmp.bmHeight,
              pixels,
              (BITMAPINFO*)&bi,
              DIB_RGB_COLORS);

    // File header
    BITMAPFILEHEADER bmfHeader;

    bmfHeader.bfType = 0x4D42; // "BM"
    bmfHeader.bfOffBits =
        sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    bmfHeader.bfSize =
        bmfHeader.bfOffBits + bmpSize;

    bmfHeader.bfReserved1 = 0;
    bmfHeader.bfReserved2 = 0;

    // Open file
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    // Write headers + pixels
    DWORD written;

    WriteFile(hFile,
              &bmfHeader,
              sizeof(BITMAPFILEHEADER),
              &written,
              NULL);

    WriteFile(hFile,
              &bi,
              sizeof(BITMAPINFOHEADER),
              &written,
              NULL);

    WriteFile(hFile,
              pixels,
              bmpSize,
              &written,
              NULL);

    // Cleanup
    CloseHandle(hFile);

    delete[] pixels;

    DeleteObject(hBitmap);

    DeleteDC(hdcMem);

    ReleaseDC(hwnd, hdcWindow);
}

void LoadBMP(HWND hwnd, const char* filename)
{
    // Open BMP file
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE)
        return;

    DWORD bytesRead;

    // Read file header
    BITMAPFILEHEADER bmfHeader;
    ReadFile(
        hFile,
        &bmfHeader,
        sizeof(BITMAPFILEHEADER),
        &bytesRead,
        NULL
    );

    // Check if file is BMP
    if (bmfHeader.bfType != 0x4D42)
    {
        CloseHandle(hFile);
        return;
    }

    // Read info header
    BITMAPINFOHEADER bi;
    ReadFile(
        hFile,
        &bi,
        sizeof(BITMAPINFOHEADER),
        &bytesRead,
        NULL
    );

    // Calculate image size
    DWORD bmpSize =
        ((bi.biWidth * bi.biBitCount + 31) / 32) * 4 * bi.biHeight;

    // Allocate memory for pixels
    char* pixels = new char[bmpSize];

    // Move file pointer to pixel data
    SetFilePointer(
        hFile,
        bmfHeader.bfOffBits,
        NULL,
        FILE_BEGIN
    );

    // Read pixel data
    ReadFile(
        hFile,
        pixels,
        bmpSize,
        &bytesRead,
        NULL
    );

    // Close file
    CloseHandle(hFile);

    // Get window DC
    HDC hdcWindow = GetDC(hwnd);

    // Create bitmap from pixel data
    HBITMAP hBitmap = CreateDIBitmap(
        hdcWindow,
        &bi,
        CBM_INIT,
        pixels,
        (BITMAPINFO*)&bi,
        DIB_RGB_COLORS
    );

    // Create memory DC
    HDC hdcMem = CreateCompatibleDC(hdcWindow);

    // Select bitmap into memory DC
    SelectObject(hdcMem, hBitmap);

    // Draw bitmap to window
    BitBlt(
        hdcWindow,
        0,
        0,
        bi.biWidth,
        bi.biHeight,
        hdcMem,
        0,
        0,
        SRCCOPY
    );

    // Cleanup
    delete[] pixels;

    DeleteObject(hBitmap);

    DeleteDC(hdcMem);

    ReleaseDC(hwnd, hdcWindow);
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
                case IDM_SAVE:
                {
                    char filename[MAX_PATH];

                    OPENFILENAME ofn;
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFile = filename;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrFilter = "Bitmap Files\0*.bmp\0All Files\0*.*\0";
                    ofn.lpstrDefExt = "bmp";
                    ofn.Flags = OFN_OVERWRITEPROMPT;

                    if (GetSaveFileName(&ofn))
                    {
                        string name = filename;

                        if (name.find(".bmp") == string::npos)
                        {
                            name += ".bmp";
                        }

                        SaveBMP(hwnd, name.c_str());
                    }
                    break;
                }

                case IDM_LOAD:
                {
                    char filename[MAX_PATH];

                    OPENFILENAME ofn;
                    ZeroMemory(&ofn, sizeof(ofn));

                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFile = filename;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrFilter =
                        "Bitmap Files\0*.bmp\0"
                        "All Files\0*.*\0";
                    ofn.lpstrDefExt = "bmp";
                    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

                    if (GetOpenFileName(&ofn))
                    {
                        LoadBMP(hwnd, filename);
                    }
                    break;
                }

                case IDM_CLEAR:
                    DrawnCircles.clear();
                    DrawnEllipses.clear();
                    ClearScreen(hwnd);
                    break;

                case IDM_BG_WHITE:
                {
                    HDC hdcWin = GetDC(hwnd);
                    RECT rect;
                    GetClientRect(hwnd, &rect);
                    int w = rect.right, h = rect.bottom;

                    HDC hdcMem = CreateCompatibleDC(hdcWin);
                    HBITMAP hBmp = CreateCompatibleBitmap(hdcWin, w, h);
                    SelectObject(hdcMem, hBmp);
                    BitBlt(hdcMem, 0, 0, w, h, hdcWin, 0, 0, SRCCOPY);

                    HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
                    FillRect(hdcWin, &rect, white);
                    DeleteObject(white);

                    COLORREF bgColor = GetPixel(hdcMem, 0, h-1);
                    for(int x = 0; x < w; x++)
                        for(int y = 0; y < h; y++)
                        {
                            COLORREF px = GetPixel(hdcMem, x, y);
                            if(px != bgColor)
                                SetPixel(hdcWin, x, y, px);
                        }

                    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND,
                                    (LONG_PTR)GetStockObject(WHITE_BRUSH));

                    DeleteObject(hBmp);
                    DeleteDC(hdcMem);
                    ReleaseDC(hwnd, hdcWin);
                    break;
                }

                case IDM_CURSOR_CROSS:
                SetClassLongPtr(hwnd, GCLP_HCURSOR,
                (LONG_PTR)LoadCursor(NULL, IDC_CROSS));
                break;

                case IDM_CURSOR_HAND:
                SetClassLongPtr(hwnd, GCLP_HCURSOR,
                (LONG_PTR)LoadCursor(NULL, IDC_HAND));
                break;

                case IDM_COLOR_RED:    CurrentColor = RGB(255, 0,   0);   break;
                case IDM_COLOR_GREEN:  CurrentColor = RGB(0,   200, 0);   break;
                case IDM_COLOR_BLUE:   CurrentColor = RGB(0,   0,   255); break;
                case IDM_COLOR_BLACK:  CurrentColor = RGB(0,   0,   0);   break;
                case IDM_COLOR_YELLOW: CurrentColor = RGB(255, 255, 0);   break;

                case IDM_DDA:
                    CurrentMode = LINE_DDA;
                    break;

                case IDM_MIDPOINT:
                    CurrentMode = LINE_MIDPOINT;
                    break;

                case IDM_PARAMETRIC:
                    CurrentMode = LINE_PARAMETRIC;
                    break;

                case IDM_CIRCLE_DIRECT:
                    CurrentMode = CIRCLE_DIRECT;
                    break;

                case IDM_CIRCLE_POLAR:
                    CurrentMode = CIRCLE_POLAR;
                    break;

                case IDM_CIRCLE_ITERATIVE:
                    CurrentMode = CIRCLE_ITERATIVE_POLAR;
                    break;

                case IDM_CIRCLE_MIDPOINT:
                    CurrentMode = CIRCLE_MIDPOINT;
                    break;

                case IDM_CIRCLE_MODIFIED:
                    CurrentMode = CIRCLE_MODIFIED;
                    break;

                case IDM_ELLIPSE_DIRECT:
                    CurrentMode = ELLIPSE_DIRECT;
                    break;

                case IDM_ELLIPSE_POLAR:
                    CurrentMode = ELLIPSE_POLAR;
                    break;

                case IDM_ELLIPSE_MIDPOINT:
                    CurrentMode = ELLIPSE_MIDPOINT;
                    break;

                case IDM_CARDINAL:
                    CurrentMode = CARDINAL_SPLINE;
                    TempPoints.clear();
                    break;
                case IDM_FILL_RECT_BEZIER:
                    CurrentMode = FILL_RECT_BEZIER;
                    TempPoints.clear();
                    break;

                case IDM_CLIP_SQ_POINT:
                    CurrentMode = CLIP_SQUARE_POINT;
                    TempPoints.clear();
                    break;

                case IDM_CLIP_SQ_LINE:
                    CurrentMode = CLIP_SQUARE_LINE;
                    TempPoints.clear();
                    break;

                case IDM_HAPPY:
                    CurrentMode = HAPPY_FACE;
                    break;

                case IDM_SAD:
                    CurrentMode = SAD_FACE;
                    break;

                case IDM_FILL_CIRCLE_LINES:
                    CurrentMode = FILL_CIRCLE_LINES;
                    TempPoints.clear();
                    break;
                
                case IDM_FILL_CIRCLE_CIRCLES:
                    CurrentMode = FILL_CIRCLE_CIRCLES;
                    break;

                case IDM_FILL_SQUARE_HERMITE:
                    CurrentMode = FILL_SQUARE_HERMITE;
                    TempPoints.clear();
                    break;

                case IDM_FLOOD_FILL_RECURSIVE:
                    CurrentMode = FLOOD_FILL_RECURSIVE;
                    TempPoints.clear();
                    break;
                case IDM_FLOOD_FILL_NON_RECURSIVE:
                    CurrentMode = FLOOD_FILL_NON_RECURSIVE;
                    TempPoints.clear();
                    break;
            }

            break;
        }

            // ================= MOUSE DRAW =================
        case WM_LBUTTONDOWN:
            {
                TempPoints.push_back(Point(LOWORD(lp), HIWORD(lp)));

                HDC hdc = GetDC(hwnd);

                // ================= SPLINE (multi-point) =================
                if (CurrentMode == CARDINAL_SPLINE)
                {
                    if (TempPoints.size() >= 4)
                    {
                        DrawCardinalSpline(hdc, TempPoints, 0.5f, CurrentColor);
                        TempPoints.clear();
                    }

                    ReleaseDC(hwnd, hdc);
                    break;
                }
                if (CurrentMode == FLOOD_FILL_RECURSIVE)
                {
                    POINT seed = { LOWORD(lp), HIWORD(lp) };

                    Point click(seed.x, seed.y);

                    bool insideAny = false;

                    for (const auto& c : DrawnCircles)
                    {
                        if (PointInsideCircle(click, c.center, c.radius))
                        {
                            insideAny = true;
                            break;
                        }
                    }

                    if (!insideAny)
                    {
                        for (const auto& e : DrawnEllipses)
                        {
                            if (PointInsideEllipse(click, e.center, e.a, e.b))
                            {
                                insideAny = true;
                                break;
                            }
                        }
                    }

                    if (!insideAny)
                    {
                        MessageBox(hwnd,
                                   "Click inside a circle or ellipse only!",
                                   "Invalid Fill",
                                   MB_OK | MB_ICONWARNING);
                        ReleaseDC(hwnd, hdc);
                        return 0;
                    }

                    COLORREF oldColor = GetPixel(hdc, seed.x, seed.y);
                    if (oldColor == RGB(255, 0, 0))
                    {
                        ReleaseDC(hwnd, hdc);
                        return 0;
                    }

                    FloodFillRec(hdc, seed.x, seed.y, oldColor, RGB(255, 0, 0));

                    ReleaseDC(hwnd, hdc);
                    return 0;
                }

                if (CurrentMode == FLOOD_FILL_NON_RECURSIVE) {
                    POINT center = { LOWORD(lp), HIWORD(lp) };
                    NonRecursiveFloodFill(hdc, center.x, center.y, RGB(69, 69, 69), RGB(255, 255, 255));
                    break;
                }

                if (CurrentMode == FILL_CIRCLE_LINES)
                {
                    if (step == WAIT_CENTER)
                    {
                        center = Point(LOWORD(lp), HIWORD(lp));
                        step = WAIT_RADIUS;
                    }
                    else if (step == WAIT_RADIUS)
                    {
                        radiusPoint = Point(LOWORD(lp), HIWORD(lp));
                        step = WAIT_QUARTER;
                    }
                    else if (step == WAIT_QUARTER)
                    {
                        quarterPoint = Point(LOWORD(lp), HIWORD(lp));

                        int dx = radiusPoint.x - center.x;
                        int dy = radiusPoint.y - center.y;
                        int r = (int)sqrt(dx * dx + dy * dy);

                        int quarter;

                        if (quarterPoint.x >= center.x && quarterPoint.y <= center.y)
                            quarter = 1;
                        else if (quarterPoint.x <= center.x && quarterPoint.y <= center.y)
                            quarter = 2;
                        else if (quarterPoint.x <= center.x && quarterPoint.y >= center.y)
                            quarter = 3;
                        else
                            quarter = 4;

                        // 1) draw full circle
                        CircleMidpoint(hdc, center, r, CurrentColor);

                        // 2) fill selected quarter
                        FillCircleWithLines(hdc, center, r, quarter, CurrentColor);

                        // reset
                        step = WAIT_CENTER;
                        TempPoints.clear();
                    }

                    ReleaseDC(hwnd, hdc);
                    break;
                }
                if (CurrentMode == FILL_CIRCLE_CIRCLES) {
                    // TempPoints.push_back(Point(LOWORD(lp), HIWORD(lp)));
                    SetPixel(hdc, 100, 100, RGB(255,0,0));
                    if (TempPoints.size() == 3) {
                        Point center = TempPoints[0];
                        Point boundary = TempPoints[1];
                        Point quarter = TempPoints[2];
                        int dx = boundary.x - center.x;
                        int dy = boundary.y - center.y;
                        int r = (int)sqrt(dx * dx + dy * dy);
                        CircleMidpoint(hdc, center, r, CurrentColor);
                        
                        FillCircleWithCircles(hdc, center, r, quarter, CurrentColor);
                        TempPoints.clear();   
                    }
                    ReleaseDC(hwnd, hdc);
                    break;
                }
                // ===== ELLIPSE (3 clicks: center, a-axis point, b-axis point)
                if (CurrentMode == ELLIPSE_DIRECT  ||
                    CurrentMode == ELLIPSE_POLAR   ||
                    CurrentMode == ELLIPSE_MIDPOINT)
                {
                    if (TempPoints.size() == 3)
                    {
                        Point pc = TempPoints[0]; // center
                        Point pa = TempPoints[1]; // defines a (horizontal radius)
                        Point pb = TempPoints[2]; // defines b (vertical radius)

                        int a = (int)sqrt((double)(pa.x - pc.x)*(pa.x - pc.x)
                                          + (double)(pa.y - pc.y)*(pa.y - pc.y));
                        int b = (int)sqrt((double)(pb.x - pc.x)*(pb.x - pc.x)
                                          + (double)(pb.y - pc.y)*(pb.y - pc.y));

                        switch (CurrentMode)
                        {
                            case ELLIPSE_DIRECT:
                                EllipseDirect(hdc, pc, a, b, CurrentColor);
                                DrawnEllipses.push_back({pc, a, b});
                                break;
                            case ELLIPSE_POLAR:
                                EllipsePolar(hdc, pc, a, b, CurrentColor);
                                DrawnEllipses.push_back({pc, a, b});
                                break;
                            case ELLIPSE_MIDPOINT:
                                EllipseMidpoint(hdc, pc, a, b, CurrentColor);
                                DrawnEllipses.push_back({pc, a, b});
                                break;
                        }

                        TempPoints.clear();
                    }
                    ReleaseDC(hwnd, hdc);
                    break;
                }
                // ================= 2-POINT MODES =================
                if (TempPoints.size() == 2)
                {
                    Point p1 = TempPoints[0];
                    Point p2 = TempPoints[1];

                    int dx = p2.x - p1.x;
                    int dy = p2.y - p1.y;

                    switch (CurrentMode)
                    {
                        // ===== LINES =====
                        case LINE_DDA:
                            LineDDA(hdc, p1, p2, CurrentColor);
                            break;

                        case LINE_MIDPOINT:
                            LineMidpoint(hdc, p1, p2, CurrentColor);
                            break;

                        case LINE_PARAMETRIC:
                            LineParametric(hdc, p1, p2, CurrentColor);
                            break;

                            // ===== CIRCLES =====
                        case CIRCLE_DIRECT: {
                            int r = sqrt(dx * dx + dy * dy);

                            DrawnCircles.push_back({p1, r});

                            CircleDirect(hdc, p1, sqrt(dx * dx + dy * dy), CurrentColor);
                            break;
                        }

                        case CIRCLE_POLAR: {
                            int r = sqrt(dx * dx + dy * dy);

                            DrawnCircles.push_back({p1, r});

                            CirclePolar(hdc, p1, sqrt(dx * dx + dy * dy), CurrentColor);
                            break;
                        }

                        case CIRCLE_ITERATIVE_POLAR: {
                            int r = sqrt(dx * dx + dy * dy);

                            DrawnCircles.push_back({p1, r});

                            CircleIterativePolar(hdc, p1, sqrt(dx * dx + dy * dy), CurrentColor);
                            break;
                        }

                        case CIRCLE_MIDPOINT: {
                            int r = sqrt(dx * dx + dy * dy);

                            DrawnCircles.push_back({p1, r});

                            CircleMidpoint(hdc, p1, sqrt(dx * dx + dy * dy), CurrentColor);
                            break;
                        }

                        case CIRCLE_MODIFIED: {
                            int r = sqrt(dx * dx + dy * dy);

                            DrawnCircles.push_back({p1, r});

                            CircleModifiedMidpoint(hdc, p1, sqrt(dx * dx + dy * dy), CurrentColor);
                            break;
                        }

                            // ===== FACES =====
                        case HAPPY_FACE:
                            DrawFace(hdc, p1, sqrt(dx*dx + dy*dy), HAPPY, CurrentColor);
                            break;

                        case SAD_FACE:
                            DrawFace(hdc, p1, sqrt(dx*dx + dy*dy), SAD, CurrentColor);
                            break;

                        case FILL_RECT_BEZIER:
                            FillRectangleBezier(hdc, p1, p2, CurrentColor);
                            break;

                        case FILL_SQUARE_HERMITE:
                        {
                            int side = max(abs(dx), abs(dy));

                            FillSquareWithHermite(
                                    hdc,
                                    p1.x,
                                    p1.y,
                                    side,
                                    CurrentColor
                            );

                            break;
                        }
                    }

                    TempPoints.clear();
                }

                ReleaseDC(hwnd, hdc);
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
    wc.hbrBackground = CreateSolidBrush(RGB(211, 211, 211));

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
