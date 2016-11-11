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
		unsigned GetTrianglesCount();

		Triangle GetTriangleVertices(unsigned index);
		
	};
}