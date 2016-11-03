#include <vector> 

#include "common/PolyModel.h" 
#include "common/Point.h" 
#include "common/Triangle.h" 
#include "Result.h"
#include "common\Vector3.h"
using namespace std;
using namespace rk9;

// Поиск пересечения двух многогранников 
// model_a, model_b - исходные модели 
// Возвращает координаты точек, образующих контур пересечения 
vector<Result> GetIntersectionPoints(PolyModel model_a, PolyModel model_b);
bool IsTheSamePoints(Point p1, Point p2);
void IntersectTriangles(Triangle tr1, Triangle tr2, vector<Result>& intersection_points,int i,int j);
void IntersectEdgeTriangle(Point, Point, Triangle tr1,Triangle tr2, vector<Result>& intersection_points,int i,int j);
bool IsPlaneBetweenTriangle(Triangle tr1, Triangle tr2);
void DeleteSamePoints(vector<Point>& intersection_points);
void Sort(vector<Result>&intersection_points);
void CreatePolygons(vector<Result>&intersection_points, PolyModel model_a, PolyModel model_b, vector<Polygon> &polygons_a, vector<Polygon> &polygons_b);
void DeleteSamePolygons(vector<Polygon> &polygons);
const double EPSILON = 1E-10;
int oo = 0;
int main(int argc, char ** argv) {

	PolyModel model_a, model_b;

	model_a.ReadFromSTLFile(argv[1]); 
	model_b.ReadFromSTLFile(argv[2]); 
	
	vector<Result> intersection_points = GetIntersectionPoints(model_a, model_b);
	Sort(intersection_points);
	vector<Polygon> polygons_a; //polygons of model_a
	vector<Polygon> polygons_b; //polygons of model_b
	CreatePolygons(intersection_points,model_a,model_b, polygons_a, polygons_b);
	DeleteSamePolygons(polygons_a);
	DeleteSamePolygons(polygons_b);

	

	//Из-за того, что модель состоит из треугольников, а в результате пересечения 
	//мы получаем массив с количеством точек, не обязательно кратным 3, приходится 
	//дублировать 1 или 2 точки, чтобы записать этот массив в STL-модель и отобразить рез-т
	//intersection_points.push_back(intersection_points[4]);
	//PolyModel modelz;
	//for (int i = 0; i < intersection_points.size() - 2; i += 3)
	//	modelz.AddTriangle(intersection_points[i], intersection_points[i + 1], intersection_points[i+2] );
	
	//modelz.WriteToSTLFile(argv[3]);

	return 0;
}

vector<Result> GetIntersectionPoints(PolyModel model_a, PolyModel model_b) {
	vector<Result> results;

	unsigned triangles_count_A = model_a.GetTrianglesCount();
	unsigned triangles_count_B = model_b.GetTrianglesCount();

	for (unsigned i = 0; i < triangles_count_A; i++) {
		Triangle tr1 = model_a.GetTriangleVertices(i);

		for (unsigned j = 0; j < triangles_count_B; j++) {
			Triangle tr2 = model_b.GetTriangleVertices(j);

			if (IsPlaneBetweenTriangle(tr1, tr2) ==false) {		
				oo++;
				IntersectTriangles(tr1, tr2, results,i,j);//points of intersection of 2 triangles 
			}
		}
	}
	return results;
}

//finding the intersection between the edge of one triangle and plane of another
void IntersectTriangles(Triangle tr1, Triangle tr2, vector<Result> &results,int i,int j) {
	IntersectEdgeTriangle(tr1.A, tr1.B, tr2,tr1, results, i, j); 
	IntersectEdgeTriangle(tr1.A, tr1.C, tr2,tr1, results, i, j);  
	IntersectEdgeTriangle(tr1.B, tr1.C, tr2,tr1, results, i, j);
												
	IntersectEdgeTriangle(tr2.A, tr2.B, tr1,tr2, results, i, j);
	IntersectEdgeTriangle(tr2.A, tr2.C, tr1,tr2, results, i, j);
	IntersectEdgeTriangle(tr2.B, tr2.C, tr1,tr2, results, i, j);
}


void IntersectEdgeTriangle(Point point1, Point point2, Triangle tr1,Triangle tr2, vector<Result> &results,int i,int j) {
	Plane plane = tr1.GetPlane();
	Point intersect_point;
	if (plane.GetIntersectionWithLine(point1, point2, intersect_point))
	{
		if ((tr1.IsPointInsideTriangle(intersect_point) ==true)&&// check if finded point lies in triangles
			(tr2.IsPointInsideTriangle(intersect_point) == true))  
		{
			Result res;
			res.points.push_back(intersect_point);
			res.index_i = i;
			res.index_j = j;
			results.push_back(res);
		}
	}
}

//check if there may be plane between triangles(all points of tr1 are on the one side of points of tr2) 
bool IsPlaneBetweenTriangle(Triangle tr1, Triangle tr2) {
	Vector3 v1(tr1.A, tr1.B);
	Vector3 v2(tr1.A, tr1.C);
	Vector3 norm= norm.CrossProduct(v1, v2);

	double d2 = -norm.A*tr1.A.X - norm.B*tr1.A.Y - norm.C*tr1.A.Z;

	double dv1 = norm.A*tr2.A.X + norm.B*tr2.A.Y + norm.C*tr2.A.Z + d2;
	double dv2 = norm.A*tr2.B.X + norm.B*tr2.B.Y + norm.C*tr2.B.Z + d2;
	double dv3 = norm.A*tr2.C.X + norm.B*tr2.C.Y + norm.C*tr2.C.Z + d2;
	if((dv1>=0&&dv2>0&&dv3>0)|| (dv1<0 && dv2<0 && dv3<0))

	/*if (((tr1.A.X <tr2.A.X) &&( tr1.A.Y < tr2.A.Y) &&( tr1.A.Z<tr2.A.Z)&& 
		 (tr1.A.X <tr2.B.X) &&( tr1.A.Y < tr2.B.Y) &&( tr1.A.Z<tr2.B.Z)&& 
		 (tr1.A.X <tr2.C.X) &&( tr1.A.Y < tr2.C.Y) &&( tr1.A.Z<tr2.C.Z)&& 
		 (tr1.B.X <tr2.A.X) &&( tr1.B.Y < tr2.A.Y) &&( tr1.B.Z<tr2.A.Z)&& 
		 (tr1.B.X <tr2.B.X) &&( tr1.B.Y < tr2.B.Y) &&( tr1.B.Z<tr2.B.Z)&& 
		 (tr1.B.X <tr2.C.X) &&( tr1.B.Y < tr2.C.Y) &&( tr1.B.Z<tr2.C.Z)&& 
		 (tr1.C.X <tr2.A.X) &&( tr1.C.Y < tr2.A.Y) &&( tr1.C.Z<tr2.A.Z)&& 
		 (tr1.C.X <tr2.B.X) &&( tr1.C.Y < tr2.B.Y) &&( tr1.C.Z<tr2.B.Z)&& 
		 (tr1.C.X <tr2.C.X) &&( tr1.C.Y < tr2.C.Y) &&( tr1.C.Z<tr2.C.Z))
		||		 	
		((tr1.A.X > tr2.A.X)&& (tr1.A.Y > tr2.A.Y )&& (tr1.A.Z > tr2.A.Z)&&
		 (tr1.A.X > tr2.B.X)&& (tr1.A.Y > tr2.B.Y )&& (tr1.A.Z > tr2.B.Z)&&
		 (tr1.A.X > tr2.C.X)&& (tr1.A.Y > tr2.C.Y )&& (tr1.A.Z > tr2.C.Z)&&
		 (tr1.B.X > tr2.A.X)&& (tr1.B.Y > tr2.A.Y )&& (tr1.B.Z > tr2.A.Z)&&
		 (tr1.B.X > tr2.B.X)&& (tr1.B.Y > tr2.B.Y )&& (tr1.B.Z > tr2.B.Z)&&
		 (tr1.B.X > tr2.C.X)&& (tr1.B.Y > tr2.C.Y )&& (tr1.B.Z > tr2.C.Z)&&
		 (tr1.C.X > tr2.A.X)&& (tr1.C.Y > tr2.A.Y )&& (tr1.C.Z > tr2.A.Z)&&
		 (tr1.C.X > tr2.B.X)&& (tr1.C.Y > tr2.B.Y )&& (tr1.C.Z > tr2.B.Z)&&
		 (tr1.C.X > tr2.C.X)&& (tr1.C.Y > tr2.C.Y )&& (tr1.C.Z > tr2.C.Z)))*/
		 return true;
	else return false;

}

//Check if points are the same
bool IsTheSamePoints(Point p1, Point p2)
{
	if ((p1.X == p2.X) && (p1.Y == p2.Y) && (p2.Z == p1.Z))
	{
		return true;
	}
	else return false;
}

//delete same points in array of intersection points
void DeleteSamePoints(vector<Point>& points)
{
	for (int i = 0; i<points.size() ; i++) {
		for (int j = i + 1; j < points.size(); j++)
		{
			if ((IsTheSamePoints(points[i], points[j])) == true)
			{
				points.erase(points.begin() + j);
				j--;
			}
		}
	}
}
void Sort(vector<Result>&intersection_points)
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
void CreatePolygons(vector<Result>&intersection_points,PolyModel model_a, PolyModel model_b, vector<Polygon> &polygons_a, vector<Polygon> &polygons_b)
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
					if (IsTheSamePoints(polygons[i].points[k], polygons[j].points[k]))
					{
						if (k == polygons[i].points.size()-1)
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
