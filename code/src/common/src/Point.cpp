#include "common/Point.h"

namespace rk9 {
	// Определение положения точки относительно отрезка
	double Point::Tol = 0.0000000001;
	int Point::classify(Point& p0, Point& p1)
	{
		Point p2 = *this;
		Point a = p1 - p0;
		Point b = p2 - p0;

		double sa = a.X * b.Y - b.X * a.Y;
		if (sa > Tol)
			return LEFT;
		if (sa < -Tol)
			return RIGHT;
		if ((a.X * b.X < 0.0) || (a.Y * b.Y < 0.0))
			return BEHIND;
		if (a.length() < b.length())
			return BEYOND;
		if (p0 == p2)
			return ORIGIN;
		if (p1 == p2)
			return DESTINATION;
		return BETWEEN;
	}
}