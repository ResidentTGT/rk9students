#pragma once

#include <vector>
#include "Polygon.h"
#include "common\PolyModel.h"
namespace rk9
{
	class Operation {

	public:
		static PolyModel  Add(PolyModel model_a, PolyModel model_b);
		static PolyModel Intersect(PolyModel model_a, PolyModel model_b);
		static PolyModel Substract(PolyModel model_a, PolyModel model_b);	
	};
}