#pragma once
#include <vector>
#include "common/common_api.h"
#include "common/Point.h"

using namespace std;

namespace rk9
{
	struct IntersectionResult {
		
		int index_i;//index of triangle in model a
		int index_j;//index of triangle in model b

		vector<Point> points;//intersection points of triangle i and j		
	};

	struct Polygon {
		vector<Point> points;
	};
}