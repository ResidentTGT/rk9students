#include <vector> 

#include "common/PolyModel.h" 
#include "common/Point.h" 
#include "common/Triangle.h" 
#include "IntersectionResult.h"
#include "common\Vector3.h"
#include <algorithm>
using namespace std;
using namespace rk9;

// ѕоиск пересечени€ двух многогранников 
// model_a, model_b - исходные модели 
// ¬озвращает координаты точек пересечени€ с индексами треугольников, образующих пересечение
vector<IntersectionResult> GetIntersectionPoints(PolyModel model_a, PolyModel model_b);
void IntersectTriangles(Triangle tr1, Triangle tr2, vector<IntersectionResult>& intersection_points, int i, int j);
Point IntersectEdgeTriangle(Point, Point, Triangle tr1, Triangle tr2, vector<IntersectionResult>& intersection_points, int i, int j);
bool IsPlaneBetweenTriangle(Triangle tr1, Triangle tr2);
void DeleteSamePoints(vector<Point>& intersection_points);
void DeleteSameIntersections(vector<IntersectionResult>&intersection_points);
void UniteIntersectPointsWithTriangles(vector<IntersectionResult>&intersection_points, PolyModel model_a, PolyModel model_b, vector<Polygon> &polygons_a, vector<Polygon> &polygons_b);
void DeleteSamePolygons(vector<Polygon> &polygons);
PolyModel DeleteTrianglesFromModels(PolyModel model_a, vector<IntersectionResult>&intersection_points, int i);
vector<PolyModel> DivideModels(PolyModel model_a, PolyModel model_b);
PolyModel AdditionOfModels(PolyModel model_a, PolyModel model_b);
PolyModel SubstractionModelaModelb(PolyModel model_a, PolyModel model_b);
PolyModel IntersectModels(PolyModel model_a, PolyModel model_b);

int main(int argc, char ** argv) {

	PolyModel model_a, model_b;

	model_a.ReadFromSTLFile(argv[1]);
	model_b.ReadFromSTLFile(argv[2]);

	vector<IntersectionResult> intersection_points = GetIntersectionPoints(model_a, model_b);
	DeleteSameIntersections(intersection_points);
	vector<Polygon> polygons_a; //polygons of model_a
	vector<Polygon> polygons_b; //polygons of model_b
	UniteIntersectPointsWithTriangles(intersection_points, model_a, model_b, polygons_a, polygons_b);
	DeleteSamePolygons(polygons_a);
	DeleteSamePolygons(polygons_b);

	model_a = DeleteTrianglesFromModels(model_a, intersection_points, 1);
	model_b = DeleteTrianglesFromModels(model_b, intersection_points, 2);

	//AddToModela(Triangulate(polygons_a));
	//AddToModela(Triangulate(polygons_b));
	vector<PolyModel> polymodels = DivideModels(model_a, model_b);

	PolyModel result1, result2, result3, result4;
	result1 = AdditionOfModels(polymodels[2], polymodels[3]);
	result2 = SubstractionModelaModelb(polymodels[0], polymodels[3]);
	result3 = SubstractionModelaModelb(polymodels[1], polymodels[2]);
	result4 = IntersectModels(polymodels[0], polymodels[1]);
	result1.WriteToSTLFile(argv[3]);
	result2.WriteToSTLFile(argv[4]);
	result3.WriteToSTLFile(argv[5]);
	result4.WriteToSTLFile(argv[6]);

	return 0;

}

vector<PolyModel> GetIntersectModels(PolyModel model_a, PolyModel model_b) {

	vector<PolyModel> polymodels;
	unsigned triangles_count_A = model_a.GetTrianglesCount();
	unsigned triangles_count_B = model_b.GetTrianglesCount();

	for (unsigned i = 0; i < triangles_count_A; i++) {
		Triangle tr1 = model_a.GetTriangleVertices(i);

		for (unsigned j = 0; j < triangles_count_B; j++) {
			Triangle tr2 = model_b.GetTriangleVertices(j);

			if (IsPlaneBetweenTriangle(tr1, tr2) == false)
				IntersectTriangles(tr1, tr2);//points of intersection of 2 triangles 			
		}
	}
	return polymodels;
}

//finding the intersection between the edge of one triangle and plane of another
vector<Point> IntersectTriangles(Triangle tr1, Triangle tr2) {
	vector<Point> intersect_points;
	Point intersect_point;
	if (IntersectEdgeTriangle(tr1.A, tr1.B, tr2, tr1, intersect_point))
		intersect_points.push_back(intersect_point);
	if (IntersectEdgeTriangle(tr1.A, tr1.C, tr2, tr1, intersect_point))
		intersect_points.push_back(intersect_point);
	if (IntersectEdgeTriangle(tr1.B, tr1.C, tr2, tr1, intersect_point))
		intersect_points.push_back(intersect_point);

	if (IntersectEdgeTriangle(tr2.A, tr2.B, tr1, tr2, intersect_point))
		intersect_points.push_back(intersect_point);
	if (IntersectEdgeTriangle(tr2.A, tr2.C, tr1, tr2, intersect_point))
		intersect_points.push_back(intersect_point);
	if (IntersectEdgeTriangle(tr2.B, tr2.C, tr1, tr2, intersect_point))
		intersect_points.push_back(intersect_point);

	return intersect_points;
}

//Ќаходит пересечение ребра с треугольником и записывает координаты точки пересечени€ с индексами пересекающихс€ треугольников
bool IntersectEdgeTriangle(Point point1, Point point2, Triangle tr1, Triangle tr2, Point intersect_point) {
	Plane plane = tr1.GetPlane();
	if (plane.GetIntersectionWithLine(point1, point2, intersect_point))
	{
		if (tr1.IsPointInsideTriangle(intersect_point) && tr2.IsPointInsideTriangle(intersect_point))// check if finded point lies in triangles
			return true;
	}
	return false;
}

//ѕровер€ет, может ли провести плоскость между двум€ треугольниками. ≈сли да, то пересеченеи исключаетс€
bool IsPlaneBetweenTriangle(Triangle tr1, Triangle tr2) {
	Vector3 v1(tr1.A, tr1.B);
	Vector3 v2(tr1.A, tr1.C);
	Vector3 normal = normal.CrossProduct(v1, v2);
	//dv1, dv2, dv3 - рассто€ни€ от вершин одного треугольника до плоскости другого
	double d2 = -normal.A*tr1.A.X - normal.B*tr1.A.Y - normal.C*tr1.A.Z;
	double dv1 = normal.A*tr2.A.X + normal.B*tr2.A.Y + normal.C*tr2.A.Z + d2;
	double dv2 = normal.A*tr2.B.X + normal.B*tr2.B.Y + normal.C*tr2.B.Z + d2;
	double dv3 = normal.A*tr2.C.X + normal.B*tr2.C.Y + normal.C*tr2.C.Z + d2;
	//если все рассто€ни€ не равны нулю и имеют одинаковый знак, то треугольник лежит по одну сторону от плоскости другого, пересечение исключаетс€
	return ((dv1 > 0 && dv2 > 0 && dv3 > 0) || (dv1 < 0 && dv2 < 0 && dv3 < 0));

}


//delete same points in array of intersection points
void DeleteSamePoints(vector<Point>& points)
{
	for (int i = 0; i < points.size(); i++) {
		for (int j = i + 1; j < points.size(); j++)
		{
			if (points[i] == points[j])
			{
				points.erase(points.begin() + j);
				j--;
			}
		}
	}
}
void DeleteSameIntersections(vector<IntersectionResult>&intersection_points)
{
	for (int i = 0; i < intersection_points.size(); i++)
	{
		for (int j = i + 1; j < intersection_points.size(); j++)
		{
			if ((intersection_points[i].index_i == intersection_points[j].index_i) && (intersection_points[i].index_j == intersection_points[j].index_j))
			{
				intersection_points[i].points.push_back(intersection_points[j].points[0]);
				intersection_points.erase(intersection_points.begin() + j);
				j--;
			}
		}
	}
}
void UniteIntersectPointsWithTriangles(vector<IntersectionResult>&intersection_points, PolyModel model_a, PolyModel model_b, vector<Polygon> &polygons_a, vector<Polygon> &polygons_b)
{

	for (int i = 0; i < intersection_points.size(); i++)
	{
		Polygon polygon_a;
		Polygon polygon_b;
		for (int j = 0; j < intersection_points[i].points.size(); j++)
		{
			polygon_a.points.push_back(intersection_points[i].points[j]);
			polygon_b.points.push_back(intersection_points[i].points[j]);
		}
		Triangle a = model_a.GetTriangleVertices(intersection_points[i].index_i);
		polygon_a.points.push_back(a.A);
		polygon_a.points.push_back(a.B);
		polygon_a.points.push_back(a.C);
		polygons_a.push_back(polygon_a);

		Triangle b = model_b.GetTriangleVertices(intersection_points[i].index_j);
		polygon_b.points.push_back(b.A);
		polygon_b.points.push_back(b.B);
		polygon_b.points.push_back(b.C);
		polygons_b.push_back(polygon_b);
	}
}

void DeleteSamePolygons(vector<Polygon> &polygons)
{
	for (int i = 0; i < polygons.size(); i++)
	{
		for (int j = i + 1; j < polygons.size(); j++)
		{
			if (polygons[i].points.size() == polygons[j].points.size())
			{
				for (int k = 0; k < polygons[i].points.size(); k++)
				{
					if (polygons[i].points[k] == polygons[j].points[k])
					{
						if (k == polygons[i].points.size() - 1)
						{
							polygons.erase(polygons.begin() + j);
							j--;
						}
						else continue;

					}
				}
			}
		}

	}
}

PolyModel DeleteTrianglesFromModels(PolyModel model, vector<IntersectionResult>& intersection_points, int i)
{
	if (i == 1) {
		vector<int> tr_indexes_model;

		for (int i = 0; i < intersection_points.size(); i++)
		{
			tr_indexes_model.push_back(intersection_points[i].index_i);

		}

		for (int i = 0; i < tr_indexes_model.size(); i++)
		{
			for (int j = i + 1; j < tr_indexes_model.size(); j++)
			{
				if (tr_indexes_model[i] == tr_indexes_model[j])
				{
					tr_indexes_model.erase(tr_indexes_model.begin() + j);
					j--;
				}
			}
		}


		sort(tr_indexes_model.begin(), tr_indexes_model.end());

		for (int i = tr_indexes_model.size() - 1; i > -1; i--)
		{
			model.DeleteTriangle(tr_indexes_model[i]);
		}
		return model;
	}
	else
	{
		vector<int> tr_indexes_model;

		for (int i = 0; i < intersection_points.size(); i++)
		{
			tr_indexes_model.push_back(intersection_points[i].index_j);

		}

		for (int i = 0; i < tr_indexes_model.size(); i++)
		{
			for (int j = i + 1; j < tr_indexes_model.size(); j++)
			{
				if (tr_indexes_model[i] == tr_indexes_model[j])
				{
					tr_indexes_model.erase(tr_indexes_model.begin() + j);
					j--;
				}
			}
		}


		sort(tr_indexes_model.begin(), tr_indexes_model.end());

		for (int i = tr_indexes_model.size() - 1; i > -1; i--)
		{
			model.DeleteTriangle(tr_indexes_model[i]);
		}
		return model;
	}
}

vector<PolyModel> DivideModels(PolyModel model_a, PolyModel model_b)
{
	vector<PolyModel> polymodels;
	PolyModel differ1, differ2;
	PolyModel model_c = model_a;
	PolyModel model_d = model_b;
	int del = 0;
	for (int i = 0; i < model_a.GetTrianglesCount(); i++)
	{

		int key = 0;
		Triangle tr = model_a.GetTriangleVertices(i);
		Vector3 v1(tr.A, tr.B);
		Vector3 v2(tr.A, tr.C);
		Vector3 normal = normal.CrossProduct(v1, v2);
		Point p1((tr.A.X + tr.B.X + tr.C.X) / 3, (tr.A.Y + tr.B.Y + tr.C.Y) / 3, (tr.A.Z + tr.B.Z + tr.C.Z) / 3);
		Point p2(p1.X + normal.A, p1.Y + normal.B, p1.Z + normal.C);
		for (int j = 0; j < model_b.GetTrianglesCount(); j++)
		{

			Point intersect_point;
			Triangle trr = model_b.GetTriangleVertices(j);
			Plane plane = trr.GetPlane();
			if (plane.GetIntersectionWithLine(p1, p2, intersect_point) && (intersect_point.X > p1.X) && trr.IsPointInsideTriangle(intersect_point))
			{

				key++;
			}
		}
		if (key % 2 != 0)
		{
			differ1.AddTriangle(tr);
			model_c.DeleteTriangle(i - del);
			del++;
		}
		key = 0;

	}
	del = 0;
	for (int i = 0; i < model_b.GetTrianglesCount(); i++)
	{

		int key = 0;
		Triangle tr = model_b.GetTriangleVertices(i);
		Vector3 v1(tr.A, tr.B);
		Vector3 v2(tr.A, tr.C);
		Vector3 normal = normal.CrossProduct(v1, v2);
		Point p1(tr.A.X, tr.A.Y, tr.A.Z);
		Point p2(tr.A.X + normal.A, tr.A.Y + normal.B, tr.A.Z + normal.C);
		for (int j = 0; j < model_a.GetTrianglesCount(); j++)
		{

			Point intersect_point;
			Triangle trr = model_a.GetTriangleVertices(j);
			Plane plane = trr.GetPlane();
			if (plane.GetIntersectionWithLine(p1, p2, intersect_point) && (intersect_point.X > p1.X) && trr.IsPointInsideTriangle(intersect_point))
			{

				key++;
			}

		}
		if (key % 2 != 0)
		{
			differ2.AddTriangle(tr);
			model_d.DeleteTriangle(i - del);
			del++;
		}
		key = 0;
	}
	polymodels.push_back(differ1);
	polymodels.push_back(differ2);
	polymodels.push_back(model_c);
	polymodels.push_back(model_d);
	return polymodels;
}
PolyModel IntersectModels(PolyModel model_a, PolyModel model_b)
{
	PolyModel c;
	for (int i = 0; i < model_a.GetTrianglesCount(); i++)
	{
		c.AddTriangle(model_a.GetTriangleVertices(i));
	}
	for (int i = 0; i < model_b.GetTrianglesCount(); i++)
	{
		c.AddTriangle(model_b.GetTriangleVertices(i));
	}
	return c;
}
PolyModel AdditionOfModels(PolyModel model_a, PolyModel model_b)
{
	PolyModel c;
	for (int i = 0; i < model_a.GetTrianglesCount(); i++)
	{
		c.AddTriangle(model_a.GetTriangleVertices(i));
	}
	for (int i = 0; i < model_b.GetTrianglesCount(); i++)
	{
		c.AddTriangle(model_b.GetTriangleVertices(i));
	}
	return c;
}

PolyModel SubstractionModelaModelb(PolyModel model_a, PolyModel model_b)
{
	PolyModel c;
	for (int i = 0; i < model_a.GetTrianglesCount(); i++)
	{
		c.AddTriangle(model_a.GetTriangleVertices(i));
	}
	for (int i = 0; i < model_b.GetTrianglesCount(); i++)
	{
		c.AddTriangle(model_b.GetTriangleVertices(i));
	}
	return c;
}


