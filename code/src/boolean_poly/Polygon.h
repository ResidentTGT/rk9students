#pragma once
#include <vector>
#include "common/common_api.h"
#include "common/Point.h"

using namespace std;

namespace rk9
{
	struct Polygon {
		vector<Point> points;
		int triangle_index;
		void AddPointsToPolygon(vector<Point>& points);
	};
}