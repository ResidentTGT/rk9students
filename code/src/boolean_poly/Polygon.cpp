#include "Polygon.h"


namespace rk9
{
	void Polygon::AddPointsToPolygon(vector<Point>& intersection_points) {
		for (int i = 0; i < intersection_points.size(); i++)
			points.push_back(intersection_points[i]);
	};
}