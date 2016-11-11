#pragma once
#include <vector>
#include "common/common_api.h"
#include "common/Point.h"

using namespace std;

namespace rk9
{
	struct Result {
		
		unsigned index_i;
		unsigned index_j;
		vector<Point> points;		
	};

	struct Polygon {
		vector<Point> points;
	};
}