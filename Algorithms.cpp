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

//============== CURVE
void DrawCardinalSpline(HDC hdc, const vector<Point> &pts, float tension, COLORREF c) {
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
