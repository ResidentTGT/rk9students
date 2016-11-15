#include <vector> 

#include "common/PolyModel.h" 
#include "common/Point.h" 
#include "common/Triangle.h" 
#include "IntersectionResult.h"
#include "common\Vector3.h"
using namespace std;
using namespace rk9;

// ѕоиск пересечени€ двух многогранников 
// model_a, model_b - исходные модели 
// ¬озвращает координаты точек пересечени€ с индексами треугольников, образующих пересечение
vector<IntersectionResult> GetIntersectionPoints(PolyModel model_a, PolyModel model_b);
void IntersectTriangles(Triangle tr1, Triangle tr2, vector<IntersectionResult>& intersection_points, int i, int j);
void IntersectEdgeTriangle(Point, Point, Triangle tr1, Triangle tr2, vector<IntersectionResult>& intersection_points, int i, int j);
bool IsPlaneBetweenTriangle(Triangle tr1, Triangle tr2);
void DeleteSamePoints(vector<Point>& intersection_points);
void DeleteSameIntersections(vector<IntersectionResult>&intersection_points);
void UniteIntersectPointsWithTriangles(vector<IntersectionResult>&intersection_points, PolyModel model_a, PolyModel model_b, vector<Polygon> &polygons_a, vector<Polygon> &polygons_b);
void DeleteSamePolygons(vector<Polygon> &polygons);

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



	//»з-за того, что модель состоит из треугольников, а в результате пересечени€ 
	//мы получаем массив с количеством точек, не об€зательно кратным 3, приходитс€ 
	//дублировать 1 или 2 точки, чтобы записать этот массив в STL-модель и отобразить рез-т
	//intersection_points.push_back(intersection_points[4]);
	//PolyModel modelz;
	//for (int i = 0; i < intersection_points.size() - 2; i += 3)
	//	modelz.AddTriangle(intersection_points[i], intersection_points[i + 1], intersection_points[i+2] );

	//modelz.WriteToSTLFile(argv[3]);

	return 0;
}

vector<IntersectionResult> GetIntersectionPoints(PolyModel model_a, PolyModel model_b) {
	vector<IntersectionResult> IntersectionResults;

	unsigned triangles_count_A = model_a.GetTrianglesCount();
	unsigned triangles_count_B = model_b.GetTrianglesCount();

	for (unsigned i = 0; i < triangles_count_A; i++) {
		Triangle tr1 = model_a.GetTriangleVertices(i);

		for (unsigned j = 0; j < triangles_count_B; j++) {
			Triangle tr2 = model_b.GetTriangleVertices(j);

			if (IsPlaneBetweenTriangle(tr1, tr2) == false)
				IntersectTriangles(tr1, tr2, IntersectionResults, i, j);//points of intersection of 2 triangles 			
		}
	}
	return IntersectionResults;
}

//finding the intersection between the edge of one triangle and plane of another
void IntersectTriangles(Triangle tr1, Triangle tr2, vector<IntersectionResult> &IntersectionResults, int index_i, int index_j) {
	IntersectEdgeTriangle(tr1.A, tr1.B, tr2, tr1, IntersectionResults, index_i, index_j);
	IntersectEdgeTriangle(tr1.A, tr1.C, tr2, tr1, IntersectionResults, index_i, index_j);
	IntersectEdgeTriangle(tr1.B, tr1.C, tr2, tr1, IntersectionResults, index_i, index_j);

	IntersectEdgeTriangle(tr2.A, tr2.B, tr1, tr2, IntersectionResults, index_i, index_j);
	IntersectEdgeTriangle(tr2.A, tr2.C, tr1, tr2, IntersectionResults, index_i, index_j);
	IntersectEdgeTriangle(tr2.B, tr2.C, tr1, tr2, IntersectionResults, index_i, index_j);
}

//Ќаходит пересечение ребра с треугольником и записывает координаты точки пересечени€ с индексами пересекающихс€ треугольников
void IntersectEdgeTriangle(Point point1, Point point2, Triangle tr1, Triangle tr2, vector<IntersectionResult> &IntersectionResults, int index_i, int index_j) {
	Plane plane = tr1.GetPlane();
	Point intersect_point;
	if (plane.GetIntersectionWithLine(point1, point2, intersect_point))
	{
		if (tr1.IsPointInsideTriangle(intersect_point) && tr2.IsPointInsideTriangle(intersect_point))// check if finded point lies in triangles
		{
			IntersectionResult result;
			result.points.push_back(intersect_point);
			result.index_i = index_i;
			result.index_j = index_j;
			IntersectionResults.push_back(result);
		}
	}
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
	((dv1 > 0 && dv2 > 0 && dv3 > 0) || (dv1 < 0 && dv2 < 0 && dv3 < 0)) ? return true : return false;
		
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
