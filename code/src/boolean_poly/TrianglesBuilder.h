#pragma once
#include <vector>
#include "common/Point.h"
#include "common/Triangle.h"
#include "Edge.h"
#include <set>
#include <iostream>
#include "Polygon.h"


// ласс содержит реализации алгоритмов 2D триангул€ции
using namespace std;
namespace rk9 {
	class TrianglesBuilder 
	{
	private:
		double d_;
		Point normal_;
		vector<Point> swapXZ(vector<Point> &s);
		Point swapXZ(Point &p);
		vector<Point> swapYZ(vector<Point> &s);
		Point swapYZ(Point &p);
		vector<Point> swapXY(vector<Point> &s);
		Point swapXY(Point &p);
		void checkPoints(Point normal, double d, vector<Point> &s);
		Point getNormal(vector<Point> &s, double &orientation);
		Point getNormal(vector<Point> &s);
		int classifyNormal(Point n);
		void modifyPoints(vector<Point> &s, int normal_type, double &orientation);
		Point InvPointTransform(Point p, int normal_type);

	public:
		TrianglesBuilder();
		//строим триангул€цию ƒелонэ инкрементным методом
		void buildTriangles(vector<Triangle> &triangles, std::vector<Point> s);
		Edge hullEdge(vector<Point> &s, int n);
		void updateFrontier(std::set<Edge> &frontier, Point &a, Point &b);
		bool mate(Edge& e, std::vector<Point> &s, int n, Point &p);
		void static buildIntersectionArea(vector<Triangle> &triangulation, vector<Polygon> &ps);
	};
}