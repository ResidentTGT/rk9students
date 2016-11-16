#pragma once

#include <vector>

#include "common/common_api.h"

#include "Point.h"
#include "Triangle.h"


namespace rk9
{
	class COMMON_API PolyModel {
	
		std::vector<Triangle> Triangles;


	public:
		int ReadFromSTLFile(char * filename);
		int WriteToSTLFile(char * filename);

		void AddTriangle(Point p1, Point p2, Point p3);
		void AddTriangle(Triangle t);
		unsigned GetTrianglesCount();
		void DeleteTriangle(unsigned index);

		Triangle GetTriangleVertices(unsigned index);

		std::vector<Point>Points;

		static PolyModel* MergeModels(PolyModel& m1, PolyModel& m2);
	};
}