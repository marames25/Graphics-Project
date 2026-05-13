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
vector<Point> Points;
vector<Line> Lines;
vector<VertexList> Polygons;

// ================= HELPERS =================
void ClearScreen(HWND hwnd) {
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

// Curves
#define IDM_CARDINAL 40

// Faces
#define IDM_HAPPY 30
#define IDM_SAD 31

// Clipping
#define IDM_RECT_POINT 50
#define IDM_RECT_LINE 51
#define IDM_RECT_POLYGON 52
#define IDM_SQUARE_POINT 53
#define IDM_SQUARE_LINE 54
#define IDM_CIRCLE_POINT 55
#define IDM_CIRCLE_LINE 56

// Shapes
#define IDM_PENTAGON 60

// ================= MENU CREATION =================
void AddMenus(HWND hwnd) {
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

	HMENU hRectMenu = CreatePopupMenu();
	AppendMenu(hRectMenu, MF_STRING, IDM_RECT_POINT, "Point");
	AppendMenu(hRectMenu, MF_STRING, IDM_RECT_LINE, "Line");
	AppendMenu(hRectMenu, MF_STRING, IDM_RECT_POLYGON, "Polygon");

	HMENU hSquareMenu = CreatePopupMenu();
	AppendMenu(hSquareMenu, MF_STRING, IDM_SQUARE_POINT, "Point");
	AppendMenu(hSquareMenu, MF_STRING, IDM_SQUARE_LINE, "Line");

	HMENU hCircleMenu = CreatePopupMenu();
	AppendMenu(hCircleMenu, MF_STRING, IDM_CIRCLE_POINT, "Point");
	AppendMenu(hCircleMenu, MF_STRING, IDM_CIRCLE_LINE, "Line");

	HMENU hClipping = CreateMenu();
	AppendMenu(hClipping, MF_POPUP, (UINT_PTR) hRectMenu, "Rectangle");
	AppendMenu(hClipping, MF_POPUP, (UINT_PTR) hSquareMenu, "Square");
	AppendMenu(hClipping, MF_POPUP, (UINT_PTR) hCircleMenu, "Circle");

	HMENU hShapes = CreateMenu();
	AppendMenu(hShapes, MF_STRING, IDM_PENTAGON, "Pentagon");

	AppendMenu(menu, MF_POPUP, (UINT_PTR) hFile, "File");
	AppendMenu(menu, MF_POPUP, (UINT_PTR) hLines, "Lines");
	AppendMenu(menu, MF_POPUP, (UINT_PTR) hCircle, "Circles");
	AppendMenu(menu, MF_POPUP, (UINT_PTR) hFace, "Faces");
	AppendMenu(menu, MF_POPUP, (UINT_PTR) hCurves, "Curves");
	AppendMenu(menu, MF_POPUP, (UINT_PTR) hClipping, "Clipping");
	AppendMenu(menu, MF_POPUP, (UINT_PTR) hShapes, "Shapes");

	SetMenu(hwnd, menu);
}

// ================= Save & Load BMP Images =================
void SaveBMP(HWND hwnd, const char *filename) {
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
	char *pixels = new char[bmpSize];

	// Get bitmap pixels
	GetDIBits(hdcWindow,
	          hBitmap,
	          0,
	          bmp.bmHeight,
	          pixels,
	          (BITMAPINFO *) &bi,
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

void LoadBMP(HWND hwnd, const char *filename) {
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
	if (bmfHeader.bfType != 0x4D42) {
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
	char *pixels = new char[bmpSize];

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
		(BITMAPINFO *) &bi,
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
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc = GetDC(hwnd);

	switch (msg) {
		// ================= MENU =================
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			if (CurrentMode == CLIP_RECT_POINT) {
				RECT r;
				GetClientRect(hwnd, &r);

				int width = r.right - r.left;
				int height = r.bottom - r.top;

				int xleft = width * 0.25;
				int xright = width * 0.75;

				int ytop = height * 0.25;
				int ybottom = height * 0.75;

				Rectangle(hdc, xleft, ytop, xright, ybottom);

				for (auto p: Points) {
					RectanglePointClipping(hdc, p.x, p.y, xleft, xright, ybottom, ytop, CurrentColor);
				}
			}

			if (CurrentMode == CLIP_RECT_LINE) {
				RECT r;
				GetClientRect(hwnd, &r);

				int width = r.right - r.left;
				int height = r.bottom - r.top;

				int xleft = width * 0.25;
				int xright = width * 0.75;

				int ytop = height * 0.25;
				int ybottom = height * 0.75;

				Rectangle(hdc, xleft, ytop, xright, ybottom);

				for (auto l: Lines) {
					RectangleLineClipping(hdc, l.x1, l.y1, l.x2, l.y2, xleft, xright, ybottom, ytop, l.color);
				}
			}

			if (CurrentMode == CLIP_RECT_POLYGON) {
				RECT r;
				GetClientRect(hwnd, &r);

				int width = r.right - r.left;
				int height = r.bottom - r.top;

				int xleft = width * 0.25;
				int xright = width * 0.75;

				int ytop = height * 0.25;
				int ybottom = height * 0.75;

				Rectangle(hdc, xleft, ytop, xright, ybottom);

				for (auto polygon: Polygons) {
					int n = polygon.size();
					Point p[n];
					for (int i = 0; i < n; ++i) {
						p[i] = Point(polygon[i].x, polygon[i].y);
					}
					RectanglePolygonClipping(hdc, p, n, xleft, ytop, xright, ybottom, CurrentColor);
				}
			}

			if (CurrentMode == CLIP_SQUARE_POINT) {
				RECT r;
				GetClientRect(hwnd, &r);

				int width = r.right - r.left;
				int height = r.bottom - r.top;

				int size = min(width, height) * 0.25;

				int centerX = width / 2;
				int centerY = height / 2;

				int xleft = centerX - size;
				int xright = centerX + size;

				int ytop = centerY - size;
				int ybottom = centerY + size;

				Rectangle(hdc, xleft, ytop, xright, ybottom);

				for (auto p: Points) {
					SquarePointClipping(hdc, p.x, p.y, xleft, xright, ybottom, ytop, CurrentColor);
				}
			}

			if (CurrentMode == CLIP_SQUARE_LINE) {
				RECT r;
				GetClientRect(hwnd, &r);

				int width = r.right - r.left;
				int height = r.bottom - r.top;

				int size = min(width, height) * 0.25;

				int centerX = width / 2;
				int centerY = height / 2;

				int xleft = centerX - size;
				int xright = centerX + size;

				int ytop = centerY - size;
				int ybottom = centerY + size;

				Rectangle(hdc, xleft, ytop, xright, ybottom);

				for (auto l: Lines) {
					SquareLineClipping(hdc, l.x1, l.y1, l.x2, l.y2, xleft, xright, ybottom, ytop, l.color);
				}
			}

			if (CurrentMode == CLIP_CIRCLE_POINT) {
				RECT r;
				GetClientRect(hwnd, &r);

				int width = r.right - r.left;
				int height = r.bottom - r.top;

				int radius = min(width, height) * 0.25;

				int centerX = width / 2;
				int centerY = height / 2;

				Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);

				for (auto p: Points) {
					CirclePointClipping(hdc, p.x, p.y, centerX, centerY, radius, CurrentColor);
				}
			}

			if (CurrentMode == CLIP_CIRCLE_LINE) {
				RECT r;
				GetClientRect(hwnd, &r);

				int width = r.right - r.left;
				int height = r.bottom - r.top;

				int radius = min(width, height) * 0.25;

				int centerX = width / 2;
				int centerY = height / 2;

				Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);

				for (auto l: Lines) {
					CircleLineClipping(hdc, l.x1, l.y1, l.x2, l.y2, centerX, centerY, radius, CurrentColor);
				}
			}

			EndPaint(hwnd, &ps);

			break;
		}

		case WM_COMMAND: {
			switch (LOWORD(wp)) {
				case IDM_SQUARE_POINT: {
					CurrentMode = CLIP_SQUARE_POINT;
					InvalidateRect(hwnd, NULL, TRUE);
					break;
				}

				case IDM_SQUARE_LINE: {
					CurrentMode = CLIP_SQUARE_LINE;
					InvalidateRect(hwnd, NULL, TRUE);
					break;
				}

				case IDM_RECT_POINT: {
					CurrentMode = CLIP_RECT_POINT;
					InvalidateRect(hwnd, NULL, TRUE);
					break;
				}

				case IDM_RECT_LINE: {
					CurrentMode = CLIP_RECT_LINE;
					InvalidateRect(hwnd, NULL, TRUE);
					break;
				}

				case IDM_RECT_POLYGON: {
					CurrentMode = CLIP_RECT_POLYGON;
					InvalidateRect(hwnd, NULL, TRUE);
					break;
				}

				case IDM_CIRCLE_POINT: {
					CurrentMode = CLIP_CIRCLE_POINT;
					InvalidateRect(hwnd, NULL, TRUE);
					break;
				}

				case IDM_CIRCLE_LINE: {
					CurrentMode = CLIP_CIRCLE_LINE;
					InvalidateRect(hwnd, NULL, TRUE);
					break;
				}

				case IDM_SAVE: {
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

					if (GetSaveFileName(&ofn)) {
						string name = filename;

						if (name.find(".bmp") == string::npos) {
							name += ".bmp";
						}

						SaveBMP(hwnd, name.c_str());
					}
					break;
				}

				case IDM_LOAD: {
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

					if (GetOpenFileName(&ofn)) {
						LoadBMP(hwnd, filename);
					}
					break;
				}

				case IDM_PENTAGON: {
					CurrentMode = PENTAGON;
					break;
				}

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

				case IDM_CARDINAL:
					CurrentMode = CARDINAL_SPLINE;
					TempPoints.clear();
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
		case WM_LBUTTONDOWN: {
			TempPoints.push_back(Point(LOWORD(lp), HIWORD(lp)));
			Points.push_back(Point(LOWORD(lp), HIWORD(lp)));


			HDC hdc = GetDC(hwnd);

			if (CurrentMode == PENTAGON) {
				if (TempPoints.size() >= 5) {
					LineMidpoint(hdc, Point(TempPoints[0].x, TempPoints[0].y), Point(TempPoints[1].x, TempPoints[1].y),
					             CurrentColor);
					LineMidpoint(hdc, Point(TempPoints[1].x, TempPoints[1].y), Point(TempPoints[2].x, TempPoints[2].y),
					             CurrentColor);
					LineMidpoint(hdc, Point(TempPoints[2].x, TempPoints[2].y), Point(TempPoints[3].x, TempPoints[3].y),
					             CurrentColor);
					LineMidpoint(hdc, Point(TempPoints[3].x, TempPoints[3].y), Point(TempPoints[4].x, TempPoints[4].y),
					             CurrentColor);
					LineMidpoint(hdc, Point(TempPoints[4].x, TempPoints[4].y), Point(TempPoints[0].x, TempPoints[0].y),
					             CurrentColor);

					VertexList vl;
					for (int i = 0; i < 5; ++i) {
						vl.push_back(Vertex(TempPoints[i].x, TempPoints[i].y));
					}

					Polygons.push_back(vl);

					TempPoints.clear();
				}
				break;
			}

			// ================= SPLINE (multi-point) =================
			if (CurrentMode == CARDINAL_SPLINE) {
				if (TempPoints.size() >= 4) {
					DrawCardinalSpline(hdc, TempPoints, 0.5f, CurrentColor);
					TempPoints.clear();
				}

				ReleaseDC(hwnd, hdc);
				break;
			}

			// ================= 2-POINT MODES =================
			if (TempPoints.size() == 2) {
				Point p1 = TempPoints[0];
				Point p2 = TempPoints[1];

				int dx = p2.x - p1.x;
				int dy = p2.y - p1.y;

				switch (CurrentMode) {
					// ===== LINES =====
					case LINE_DDA:
						LineDDA(hdc, p1, p2, CurrentColor);
						Lines.push_back(Line(p1.x, p1.y, p2.x, p2.y, CurrentColor));
						break;

					case LINE_MIDPOINT:
						LineMidpoint(hdc, p1, p2, CurrentColor);
						Lines.push_back(Line(p1.x, p1.y, p2.x, p2.y, CurrentColor));
						break;

					case LINE_PARAMETRIC:
						LineParametric(hdc, p1, p2, CurrentColor);
						Lines.push_back(Line(p1.x, p1.y, p2.x, p2.y, CurrentColor));
						break;

					// ===== CIRCLES =====
					case CIRCLE_DIRECT:
						CircleDirect(hdc, p1, sqrt(dx * dx + dy * dy), CurrentColor);
						break;

					case CIRCLE_POLAR:
						CirclePolar(hdc, p1, sqrt(dx * dx + dy * dy), CurrentColor);
						break;

					case CIRCLE_ITERATIVE_POLAR:
						CircleIterativePolar(hdc, p1, sqrt(dx * dx + dy * dy), CurrentColor);
						break;

					case CIRCLE_MIDPOINT:
						CircleMidpoint(hdc, p1, sqrt(dx * dx + dy * dy), CurrentColor);
						break;

					case CIRCLE_MODIFIED:
						CircleModifiedMidpoint(hdc, p1, sqrt(dx * dx + dy * dy), CurrentColor);
						break;

					// ===== FACES =====
					case HAPPY_FACE:
						DrawFace(hdc, p1, sqrt(dx * dx + dy * dy), HAPPY, CurrentColor);
						break;

					case SAD_FACE:
						DrawFace(hdc, p1, sqrt(dx * dx + dy * dy), SAD, CurrentColor);
						break;
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
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProcedure;
	wc.hInstance = hInst;
	wc.lpszClassName = "DrawingApp";
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);

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
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
