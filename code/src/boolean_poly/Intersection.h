#pragma once


//#include <vector>
#include "Polygon.h"
#include "common\PolyModel.h"
namespace rk9
{
	class Intersection {

	public:
		static void IntersectModels(PolyModel model_a, PolyModel model_b, vector<Polygon>&polygons_a, vector<Polygon>&polygons_b);
		static vector<Point> IntersectTriangles(Triangle tr1, Triangle tr2);
		static bool IntersectEdgeTriangle(Point point1, Point point2, Triangle tr1, Triangle tr2, Point& intersect_point);
		static bool IsPlaneBetweenTriangle(Triangle tr1, Triangle tr2);
		static vector<PolyModel> DivideModels(PolyModel model_a, PolyModel model_b);
		static void DeletePolygons(PolyModel & model_a, PolyModel & model_b, vector<Polygon>& polygons_a, vector<Polygon>& polygons_b);
		static void AddPolygonsToModel(PolyModel & model_a, vector<Polygon>& polygons_a);
		static void UnitePolygons(vector<Polygon>& polygons, PolyModel& model);
		static PolyModel UnitePolymodels(PolyModel& model_a, PolyModel& model_b);
		static PolyModel RotateModelNormal(PolyModel &model);
	};
}