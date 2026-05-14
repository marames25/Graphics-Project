#include <windows.h>
#include <bits/stdc++.h>
#include "Algorithms.h"
//============== HELPERS
void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	SetPixel(hdc, xc - x, yc - y, c);

	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc - y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - y, yc - x, c);
}

//============== LINE
void LineDDA(HDC hdc, Point p1, Point p2, COLORREF c) {
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;

	int steps = max(abs(dx), abs(dy));

	float xInc = dx / (float) steps;
	float yInc = dy / (float) steps;

	float x = p1.x;
	float y = p1.y;

	for (int i = 0; i <= steps; i++) {
		SetPixel(hdc, round(x), round(y), c);
		x += xInc;
		y += yInc;
	}
}

void LineMidpoint(HDC hdc, Point p1, Point p2, COLORREF color) {
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
	if (dx >= dy) {
		int d = 2 * dy - dx;
		int dE = 2 * dy;
		int dNE = 2 * (dy - dx);

		for (int i = 0; i < dx; i++) {
			if (d <= 0) {
				d += dE;
				x += sx;
			} else {
				d += dNE;
				x += sx;
				y += sy;
			}

			SetPixel(hdc, x, y, color);
		}
	}
	// case 2: slope > 1 (move in y)
	else {
		int d = 2 * dx - dy;
		int dN = 2 * dx;
		int dNE = 2 * (dx - dy);

		for (int i = 0; i < dy; i++) {
			if (d <= 0) {
				d += dN;
				y += sy;
			} else {
				d += dNE;
				x += sx;
				y += sy;
			}

			SetPixel(hdc, x, y, color);
		}
	}
}

void LineParametric(HDC hdc, Point p1, Point p2, COLORREF c) {
	for (double t = 0; t <= 1; t += 0.001) {
		int x = p1.x + (p2.x - p1.x) * t;
		int y = p1.y + (p2.y - p1.y) * t;

		SetPixel(hdc, x, y, c);
	}
}

void DrawCubicBezier(HDC hdc, Point P0, Point P1, Point P2, Point P3, COLORREF c) {
	for (double t = 0; t <= 1.0; t += 0.001) {
		double mt = 1.0 - t, mt2 = mt * mt, mt3 = mt2 * mt;
		double t2 = t * t, t3 = t2 * t;

		int x = (int) (mt3 * P0.x + 3 * mt2 * t * P1.x
		               + 3 * mt * t2 * P2.x + t3 * P3.x);
		int y = (int) (mt3 * P0.y + 3 * mt2 * t * P1.y
		               + 3 * mt * t2 * P2.y + t3 * P3.y);

		SetPixel(hdc, x, y, c);
	}
}

const int INSIDE = 0; // 0000
const int LEFT = 1; // 0001
const int RIGHT = 2; // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8; // 1000
int ComputeCode(Point p, int xmin, int ymin, int xmax, int ymax) {
	int code = INSIDE;

	if (p.x < xmin) code |= LEFT;
	else if (p.x > xmax) code |= RIGHT;

	if (p.y < ymin) code |= TOP;
	else if (p.y > ymax) code |= BOTTOM;

	return code;
}

//============== CIRCLE

void CircleDirect(HDC hdc, Point center, int r, COLORREF c) {
	int xc = center.x;
	int yc = center.y;

	int x = 0;
	int R2 = r * r;

	int y = r;

	Draw8Points(hdc, xc, yc, x, y, c);

	while (x < y) {
		x++;

		y = (int) round(sqrt(R2 - x * x));

		Draw8Points(hdc, xc, yc, x, y, c);
	}
}

void CirclePolar(HDC hdc, Point center, int r, COLORREF c) {
	double theta = 0;
	double dtheta = 1.0 / r;

	Point p;
	p.x = r;
	p.y = 0;

	Draw8Points(hdc, center.x, center.y, p.x, p.y, c);

	while (p.x >= p.y) {
		theta += dtheta;

		p.x = round(r * cos(theta));
		p.y = round(r * sin(theta));

		Draw8Points(hdc, center.x, center.y, p.x, p.y, c);
	}
}

void CircleIterativePolar(HDC hdc, Point center, int r, COLORREF c) {
	double x = r;
	double y = 0;

	double dtheta = 1.0 / r;

	double cdtheta = cos(dtheta);
	double sdtheta = sin(dtheta);

	Draw8Points(hdc, center.x, center.y, (int) round(x), (int) round(y), c);

	while (x >= y) {
		double x1 = x * cdtheta - y * sdtheta;
		y = x * sdtheta + y * cdtheta;
		x = x1;

		Draw8Points(hdc, center.x, center.y,
		            (int) round(x),
		            (int) round(y),
		            c);
	}
}


void CircleMidpoint(HDC hdc, Point center, int r, COLORREF c) {
	int x = 0;
	int y = r;

	int d = 1 - r;

	Draw8Points(hdc, center.x, center.y, x, y, c);

	while (x < y) {
		if (d < 0) {
			d += 2 * x + 3;
		} else {
			d += 2 * (x - y) + 5;
			y--;
		}

		x++;

		Draw8Points(hdc, center.x, center.y, x, y, c);
	}
}

void CircleModifiedMidpoint(HDC hdc, Point center, int r, COLORREF c) {
	int x = 0;
	int y = r;

	int d = 1 - r;

	int dE = 3;
	int dSE = 5 - 2 * r;

	Draw8Points(hdc, center.x, center.y, x, y, c);

	while (x < y) {
		if (d < 0) {
			d += dE;
			dSE += 2;
		} else {
			d += dSE;
			dSE += 4;
			y--;
		}

		dE += 2;
		x++;

		Draw8Points(hdc, center.x, center.y, x, y, c);
	}
}

//============== Ellipse
bool PointInsideEllipse(Point p, Point center, int a, int b) {
	double dx = p.x - center.x;
	double dy = p.y - center.y;
	return (dx * dx) / (double) (a * a) + (dy * dy) / (double) (b * b) <= 1.0;
}

vector<EllipseData> DrawnEllipses;

void EllipseDirect(HDC hdc, Point center, int a, int b, COLORREF color) {
	// x²/a² + y²/b² = 1  →  y = b * sqrt(1 - x²/a²)
	for (int x = 0; x <= a; x++) {
		int y = (int) round(b * sqrt(1.0 - (double) (x * x) / (a * a)));
		SetPixel(hdc, center.x + x, center.y - y, color);
		SetPixel(hdc, center.x - x, center.y - y, color);
		SetPixel(hdc, center.x + x, center.y + y, color);
		SetPixel(hdc, center.x - x, center.y + y, color);
	}
	for (int y = 0; y <= b; y++) {
		int x = (int) round(a * sqrt(1.0 - (double) (y * y) / (b * b)));
		SetPixel(hdc, center.x + x, center.y - y, color);
		SetPixel(hdc, center.x - x, center.y - y, color);
		SetPixel(hdc, center.x + x, center.y + y, color);
		SetPixel(hdc, center.x - x, center.y + y, color);
	}
}

void EllipsePolar(HDC hdc, Point center, int a, int b, COLORREF color) {
	// parametric: x = a*cos(t), y = b*sin(t)
	double step = 1.0 / max(a, b);
	for (double t = 0; t <= 2 * M_PI; t += step) {
		int x = (int) round(a * cos(t));
		int y = (int) round(b * sin(t));
		SetPixel(hdc, center.x + x, center.y - y, color);
	}
}

void EllipseMidpoint(HDC hdc, Point center, int a, int b, COLORREF color) {
	long long a2 = (long long) a * a;
	long long b2 = (long long) b * b;

	auto plot = [&](int x, int y) {
		SetPixel(hdc, center.x + x, center.y - y, color);
		SetPixel(hdc, center.x - x, center.y - y, color);
		SetPixel(hdc, center.x + x, center.y + y, color);
		SetPixel(hdc, center.x - x, center.y + y, color);
	};

	int x = 0, y = b;

	// --- Region 1: slope < -1 (|dy/dx| > 1) ---
	// decision param: F(x+1, y-0.5) = b²(x+1)² + a²(y-0.5)² - a²b²
	double d1 = b2 - a2 * b + 0.25 * a2;
	double dx = 2.0 * b2 * x;
	double dy = 2.0 * a2 * y;

	while (dx < dy) {
		plot(x, y);
		x++;
		dx += 2.0 * b2;
		if (d1 < 0) {
			d1 += dx + b2;
		} else {
			y--;
			dy -= 2.0 * a2;
			d1 += dx - dy + b2;
		}
	}

	// --- Region 2: slope > -1 (|dy/dx| < 1) ---
	// decision param: F(x+0.5, y-1) = b²(x+0.5)² + a²(y-1)² - a²b²
	double d2 = b2 * (x + 0.5) * (x + 0.5)
	            + a2 * (y - 1.0) * (y - 1.0)
	            - (double) a2 * b2;

	while (y >= 0) {
		plot(x, y);
		y--;
		dy -= 2.0 * a2;
		if (d2 > 0) {
			d2 += a2 - dy;
		} else {
			x++;
			dx += 2.0 * b2;
			d2 += dx - dy + a2;
		}
	}
}

//============== CURVE
void DrawCardinalSpline(HDC hdc, const vector<Point> &pts, float tension, COLORREF c) {
	HPEN hPen = CreatePen(PS_SOLID, 1, c);
	HPEN hOldPen = (HPEN) SelectObject(hdc, hPen);
	if (pts.size() < 2) return;

	int n = (int) pts.size();
	int steps = 100;

	for (int i = 0; i < n - 1; ++i) {
		Point p0 = pts[max(0, i - 1)];
		Point p1 = pts[i];
		Point p2 = pts[i + 1];
		Point p3 = pts[min(n - 1, i + 2)];

		bool first = true;

		for (int s = 0; s <= steps; ++s) {
			float t = (float) s / steps;
			float t2 = t * t;
			float t3 = t2 * t;

			float h1 = 2 * t3 - 3 * t2 + 1;
			float h2 = -2 * t3 + 3 * t2;
			float h3 = t3 - 2 * t2 + t;
			float h4 = t3 - t2;

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

			if (first) {
				MoveToEx(hdc, (int) x, (int) y, NULL);
				first = false;
			} else {
				LineTo(hdc, (int) x, (int) y);
			}
		}
	}
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}


// ============= FILLING
void FillRectangleBezier(HDC hdc, Point p1, Point p2, COLORREF c) {
	int xmin = min(p1.x, p2.x), ymin = min(p1.y, p2.y);
	int xmax = max(p1.x, p2.x), ymax = max(p1.y, p2.y);

	for (int x = xmin; x <= xmax; x++) {
		SetPixel(hdc, x, ymin, RGB(0, 0, 0));
		SetPixel(hdc, x, ymax, RGB(0, 0, 0));
	}
	for (int y = ymin; y <= ymax; y++) {
		SetPixel(hdc, xmin, y, RGB(0, 0, 0));
		SetPixel(hdc, xmax, y, RGB(0, 0, 0));
	}

	int W = xmax - xmin;
	for (int y = ymin; y <= ymax; y++) {
		Point P0(xmin, y);
		Point P1(xmin + W / 3, y);
		Point P2(xmax - W / 3, y);
		Point P3(xmax, y);

		DrawCubicBezier(hdc, P0, P1, P2, P3, c);
	}
}


//============== HAPPY /SAD FACE
void DrawFace(HDC hdc, Point center, int r, MouthType type, COLORREF c) {
	// face
	CircleMidpoint(hdc, center, r, c);

	// eye
	CircleMidpoint(hdc, Point(center.x - r / 3, center.y - r / 3), r / 10, RGB(0, 0, 0));
	CircleMidpoint(hdc, Point(center.x + r / 3, center.y - r / 3), r / 10, RGB(0, 0, 0));

	// nose
	LineMidpoint(
		hdc,
		Point(center.x, center.y - r / 6),
		Point(center.x, center.y + r / 6),
		RGB(0, 0, 0)
	);

	// mouth
	for (double t = -1; t <= 1; t += 0.01) {
		int x = center.x + t * r / 2;

		int y;
		if (type == HAPPY)
			y = center.y + r / 3 + (t * t) * r / 3; // happy
		else
			y = center.y + r / 2 - (t * t) * r / 3; // sad

		SetPixel(hdc, x, y, RGB(0, 0, 0));
	}
}

//===========Filling

FillCircleStep step = WAIT_CENTER;
Point center;
Point radiusPoint;
Point quarterPoint;


void FillCircleWithLines(HDC hdc, Point c, int r, int quarter, COLORREF color) {
	for (int y = -r; y <= r; y++) {
		int dx = (int) sqrt(r * r - y * y);

		int xStart = -dx;
		int xEnd = dx;

		for (int x = xStart; x <= xEnd; x++) {
			bool ok = false;

			if (quarter == 1 && x >= 0 && y <= 0) ok = true;
			if (quarter == 2 && x <= 0 && y <= 0) ok = true;
			if (quarter == 3 && x <= 0 && y >= 0) ok = true;
			if (quarter == 4 && x >= 0 && y >= 0) ok = true;

			if (ok)
				SetPixel(hdc, c.x + x, c.y + y, color);
		}
	}
}

vector<CircleData> DrawnCircles;

bool PointInsideCircle(Point p, Point center, int r) {
	int dx = p.x - center.x;
	int dy = p.y - center.y;

	return (dx * dx + dy * dy) < (r * r);
}

void FloodFillRec(HDC hdc,
                  int x,
                  int y,
                  COLORREF oldColor,
                  COLORREF fillColor) {
	COLORREF current = GetPixel(hdc, x, y);

	// fill same region
	if (current != oldColor)
		return;

	if (current == fillColor)
		return;

	SetPixel(hdc, x, y, fillColor);

	FloodFillRec(hdc, x + 1, y, oldColor, fillColor);
	FloodFillRec(hdc, x - 1, y, oldColor, fillColor);
	FloodFillRec(hdc, x, y + 1, oldColor, fillColor);
	FloodFillRec(hdc, x, y - 1, oldColor, fillColor);
}

void FillSquareWithHermite(HDC hdc,
                           int x1,
                           int y1,
                           int side,
                           COLORREF c) {
	int left = x1;
	int top = y1;

	int right = left + side;
	int bottom = top + side;

	// square border
	LineDDA(hdc, Point(left, top), Point(right, top), c);
	LineDDA(hdc, Point(right, top), Point(right, bottom), c);
	LineDDA(hdc, Point(right, bottom), Point(left, bottom), c);
	LineDDA(hdc, Point(left, bottom), Point(left, top), c);

	// Hermite vertical curves
	for (int x = left; x <= right; x += 2) {
		Point P0(x, top);
		Point P1(x, bottom);

		Point T0(0, side / 2);
		Point T1(0, -side / 2);

		for (double t = 0; t <= 1; t += 0.001) {
			double t2 = t * t;
			double t3 = t2 * t;

			double h1 = 2 * t3 - 3 * t2 + 1;
			double h2 = -2 * t3 + 3 * t2;
			double h3 = t3 - 2 * t2 + t;
			double h4 = t3 - t2;

			int xh = (int) (
				h1 * P0.x +
				h2 * P1.x +
				h3 * T0.x +
				h4 * T1.x
			);

			int yh = (int) (
				h1 * P0.y +
				h2 * P1.y +
				h3 * T0.y +
				h4 * T1.y
			);

			SetPixel(hdc, xh, yh, c);
		}
	}
}


void EdgeToTable(Point p1, Point p2, vector<pair<int, int> > &table) {
	if (p1.y == p2.y) return;
	if (p1.y > p2.y) {
		swap(p1.x, p2.x);
		swap(p1.y, p2.y);
	}
	double x = p1.x, y = p1.y;
	double m = double(p2.x - p1.x) / double(p2.y - p1.y);
	while (y <= p2.y) {
		table[y].first = min((int) round(x), table[y].first);
		table[y].second = max((int) round(x), table[y].second);
		y++;
		x += m;
	}
}

void ConvexFill(HDC hdc, vector<Point> polygon, COLORREF c) {
	// initializing the table
	int height = GetSystemMetrics(SM_CYSCREEN);
	vector<pair<int, int> > table(height, {INT_MAX, INT_MIN});

	// generating edges and map them to the table
	for (int i = 0; i < polygon.size(); i++) {
		Point p1 = polygon[i], p2 = polygon[(i + 1) % polygon.size()];
		EdgeToTable(p1, p2, table);
	}

	// drawing the lines
	for (int i = 0; i < height; i++) {
		if (table[i].first == INT_MAX || table[i].second == INT_MIN) continue;
		Point p1(table[i].first, i), p2(table[i].second, i);
		LineMidpoint(hdc, p1, p2, c);
	}
}

void NonRecursiveFloodFill(HDC hdc, int x, int y, COLORREF fillColor, COLORREF boundaryColor) {
	int dx[] = {0, 0, 1, -1};
	int dy[] = {1, -1, 0, 0};

	queue<Point> q;
	q.push({x, y});

	while (!q.empty()) {
		Point pnt = q.front();
		q.pop();

		COLORREF c = GetPixel(hdc, pnt.x, pnt.y);
		if (c == boundaryColor || c == fillColor) continue;

		SetPixel(hdc, pnt.x, pnt.y, fillColor);
		for (int i = 0; i < 4; i++) {
			int Nx = pnt.x + dx[i], Ny = pnt.y + dy[i];
			q.push({Nx, Ny});
		}
	}
}

// ================= CLIPPING
void RectanglePointClipping(HDC hdc, int x, int y, int xleft, int xright, int ybottom, int ytop, COLORREF color) {
	if (x >= xleft && x <= xright && y <= ybottom && y >= ytop) {
		SetPixel(hdc, x, y, color);
	}
}

OutCode GetOutCode(double x, double y, int xleft, int xright, int ybottom, int ytop) {
	OutCode oc;
	oc.all = 0;

	if (x < xleft) {
		oc.L = 1;
	} else if (x > xright) {
		oc.R = 1;
	}

	if (y > ybottom) {
		oc.B = 1;
	} else if (y < ytop) {
		oc.T = 1;
	}

	return oc;
}

void VIntersect(double x1, double y1, double x2, double y2, int xedge, double *xi, double *yi) {
	*xi = xedge;
	*yi = (xedge - x1) * (y2 - y1) / (x2 - x1) + y1;
}

void HIntersect(double x1, double y1, double x2, double y2, int yedge, double *xi, double *yi) {
	*yi = yedge;
	*xi = (yedge - y1) * (x2 - x1) / (y2 - y1) + x1;
}

void RectangleLineClipping(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int xright, int ybottom, int ytop,
                           COLORREF color) {
	double x1 = xs, y1 = ys, x2 = xe, y2 = ye;

	OutCode oc1 = GetOutCode(x1, y1, xleft, xright, ybottom, ytop);
	OutCode oc2 = GetOutCode(x2, y2, xleft, xright, ybottom, ytop);

	while ((oc1.all | oc2.all) && !(oc1.all & oc2.all)) {
		double xi, yi;

		if (oc1.all) {
			if (oc1.L) {
				VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
			} else if (oc1.R) {
				VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
			} else if (oc1.B) {
				HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
			} else if (oc1.T) {
				HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
			}

			x1 = xi;
			y1 = yi;
			oc1 = GetOutCode(x1, y1, xleft, xright, ybottom, ytop);
		} else {
			if (oc2.L) {
				VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
			} else if (oc2.R) {
				VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
			} else if (oc2.B) {
				HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
			} else if (oc2.T) {
				HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
			}

			x2 = xi;
			y2 = yi;
			oc2 = GetOutCode(x2, y2, xleft, xright, ybottom, ytop);
		}
	}

	if (!oc1.all && !oc2.all) {
		Point p1(round(x1), round(y1)), p2(round(x2), round(y2));
		LineMidpoint(hdc, p1, p2, color);
	}
}

void SquarePointClipping(HDC hdc, int x, int y, int xleft, int xright, int ybottom, int ytop, COLORREF color) {
	RectanglePointClipping(hdc, x, y, xleft, xright, ybottom, ytop, color);
}

void SquareLineClipping(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int xright, int ybottom, int ytop,
                        COLORREF color) {
	RectangleLineClipping(hdc, xs, ys, xe, ye, xleft, xright, ybottom, ytop, color);
}

bool InLeft(Vertex &v, int edge) {
	return v.x >= edge;
}

bool InRight(Vertex &v, int edge) {
	return v.x <= edge;
}

bool InTop(Vertex &v, int edge) {
	return v.y >= edge;
}

bool InBottom(Vertex &v, int edge) {
	return v.y <= edge;
}

VertexList ClipWithEdge(VertexList p, int edge, IsInFunc In, IntersectFunc Intersect) {
	VertexList OutList;
	Vertex v1 = p[p.size() - 1];
	bool v1_in = In(v1, edge);
	for (int i = 0; i < (int) p.size(); i++) {
		Vertex v2 = p[i];
		bool v2_in = In(v2, edge);
		if (!v1_in && v2_in) {
			OutList.push_back(Intersect(v1, v2, edge));
			OutList.push_back(v2);
		} else if (v1_in && v2_in) OutList.push_back(v2);
		else if (v1_in) OutList.push_back(Intersect(v1, v2, edge));
		v1 = v2;
		v1_in = v2_in;
	}
	return OutList;
}

Vertex VIntersect(Vertex &v1, Vertex &v2, int xedge) {
	Vertex res;
	res.x = xedge;
	res.y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
	return res;
}

Vertex HIntersect(Vertex &v1, Vertex &v2, int yedge) {
	Vertex res;
	res.y = yedge;
	res.x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
	return res;
}

void RectanglePolygonClipping(HDC hdc, Point *p, int n, int xleft, int ytop, int xright, int ybottom, COLORREF color) {
	VertexList vlist;
	for (int i = 0; i < n; i++) {
		vlist.push_back(Vertex(p[i].x, p[i].y));
	}
	vlist = ClipWithEdge(vlist, xleft, InLeft, VIntersect);
	vlist = ClipWithEdge(vlist, ytop, InTop, HIntersect);
	vlist = ClipWithEdge(vlist, xright, InRight, VIntersect);
	vlist = ClipWithEdge(vlist, ybottom, InBottom, HIntersect);
	Vertex v1 = vlist[vlist.size() - 1];
	for (int i = 0; i < (int) vlist.size(); i++) {
		Vertex v2 = vlist[i];
		Point p1(round(v1.x), round(v1.y)), p2(round(v2.x), round(v2.y));
		LineMidpoint(hdc, p1, p2, color);
		v1 = v2;
	}
}

void CirclePointClipping(HDC hdc, int x, int y, int cx, int cy, int r, COLORREF color) {
	int dx = x - cx;
	int dy = y - cy;

	if (dx * dx + dy * dy <= r * r) {
		SetPixel(hdc, x, y, color);
	}
}

void CircleLineClipping(HDC hdc, int xs, int ys, int xe, int ye, int cx, int cy, int r, COLORREF color) {
	double dx = xe - xs;
	double dy = ye - ys;

	double fx = xs - cx;
	double fy = ys - cy;

	double a = dx * dx + dy * dy;
	double b = 2 * (fx * dx + fy * dy);
	double c = (fx * fx + fy * fy) - r * r;

	double discriminant = b * b - 4 * a * c;

	// no intersection
	if (discriminant < 0)
		return;

	discriminant = sqrt(discriminant);

	double t1 = (-b - discriminant) / (2 * a);
	double t2 = (-b + discriminant) / (2 * a);

	// clamp to segment
	t1 = max(0.0, min(1.0, t1));
	t2 = max(0.0, min(1.0, t2));

	double x1 = xs + t1 * dx;
	double y1 = ys + t1 * dy;

	double x2 = xs + t2 * dx;
	double y2 = ys + t2 * dy;

	Point p1(round(x1), round(y1));
	Point p2(round(x2), round(y2));

	LineMidpoint(hdc, p1, p2, color);
}
