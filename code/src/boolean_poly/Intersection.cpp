#include "Operation.h"
#include <algorithm>
#include "Intersection.h"

using namespace std;

namespace rk9
{
	void Intersection::IntersectModels(PolyModel model_a, PolyModel model_b, vector<Polygon>&polygons_a, vector<Polygon>&polygons_b) {
		unsigned triangles_count_A = model_a.GetTrianglesCount();
		unsigned triangles_count_B = model_b.GetTrianglesCount();

		for (unsigned i = 0; i < triangles_count_A; i++) {
			Triangle tr1 = model_a.GetTriangleVertices(i);
			vector<Point> intersection_points;

			for (unsigned j = 0; j < triangles_count_B; j++) {
				Triangle tr2 = model_b.GetTriangleVertices(j);
				if (!IsPlaneBetweenTriangle(tr1, tr2)) {
					intersection_points = IntersectTriangles(tr1, tr2);

					if (intersection_points.size() != 0)
					{
						Polygon a, b;
						a.triangle_index = i;
						b.triangle_index = j;
						a.AddPointsToPolygon(intersection_points);
						b.AddPointsToPolygon(intersection_points);
						polygons_a.push_back(a);
						polygons_b.push_back(b);
					}
				}
			}
		}

		UnitePolygons(polygons_a, model_a);
		UnitePolygons(polygons_b, model_b);
	}

	/*bool CompareWithEps(Point p1, Point p2)
	{
		double Eps = 0.00001;
		return(abs(p1.X - p2.X) < Eps && abs(p1.Y - p2.Y) < Eps && abs(p1.Z - p2.Z) < Eps);
	}*/

	//finding the intersection between the edge of one triangle and plane of another
	vector<Point> Intersection::IntersectTriangles(Triangle tr1, Triangle tr2) {
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

	//Ќаходит пересечение ребра с треугольником 
	bool Intersection::IntersectEdgeTriangle(Point point1, Point point2, Triangle tr1, Triangle tr2, Point& intersect_point) {
		Plane plane = tr1.GetPlane();
		if (plane.GetIntersectionWithLine(point1, point2, intersect_point))
		{
			if (tr1.IsPointInsideTriangle(intersect_point) && tr2.IsPointInsideTriangle(intersect_point))// check if finded point lies in triangles
				return true;
		}
		return false;
	}

	//ѕровер€ет, может ли провести плоскость между двум€ треугольниками. ≈сли да, то пересечение исключаетс€
	bool Intersection::IsPlaneBetweenTriangle(Triangle tr1, Triangle tr2) {
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

	vector<PolyModel> Intersection::DivideModels(PolyModel model_a, PolyModel model_b)
	{
		vector<PolyModel> polymodels;
		// внутренние части моделей
		PolyModel differ1, differ2;
		// внешние части моделей
		PolyModel model_c = model_a;
		PolyModel model_d = model_b;
		int triangles_number_a = model_a.GetTrianglesCount();
		int triangles_number_b = model_b.GetTrianglesCount();

		vector<int> index_c;
		for (int i = 0; i < triangles_number_a; i++)
		{
			int key = 0;
			Triangle tr1 = model_a.GetTriangleVertices(i);
			Vector3 v1(tr1.A, tr1.B);
			Vector3 v2(tr1.A, tr1.C);
			Vector3 normal = Vector3::CrossProduct(v1, v2);
			Point p1((tr1.A.X + tr1.B.X + tr1.C.X) / 3, (tr1.A.Y + tr1.B.Y + tr1.C.Y) / 3, (tr1.A.Z + tr1.B.Z + tr1.C.Z) / 3);
			Point p2(p1.X + normal.A, p1.Y + normal.B, p1.Z + normal.C);
			vector<Point> pps;
			for (int j = 0; j < triangles_number_b; j++)
			{
				Point intersect_point;
				Triangle tr2 = model_b.GetTriangleVertices(j);
				Plane plane = tr2.GetPlane();
				if (plane.GetIntersectionWithLine(p1, p2, intersect_point) &&
					Vector3::DotProduct(Vector3(intersect_point, p1), Vector3(p2, p1)) > 0 &&
					tr2.IsPointInsideTriangle(intersect_point))
				{
					pps.push_back(intersect_point);
				}
			}

			for (int j = 0; j < pps.size(); j++)
			{
				for (int k = j + 1; k < pps.size(); k++)
				{
					if (pps[j] == pps[k])
					{
						pps.erase(pps.begin() + k);
						k--;
					}
				}
			}


			if (pps.size() % 2 != 0)
			{
				differ1.AddTriangle(tr1);
				index_c.push_back(i);
			}
			key = 0;
		}
		sort(index_c.begin(), index_c.end());
		for (int i = index_c.size() - 1; i > -1; i--)
			model_c.DeleteTriangle(index_c[i]);

		vector<int> index_d;
		for (int i = 0; i < triangles_number_b; i++)
		{
			int key = 0;
			Triangle tr1 = model_b.GetTriangleVertices(i);
			Vector3 v1(tr1.A, tr1.B);
			Vector3 v2(tr1.A, tr1.C);
			Vector3 normal = Vector3::CrossProduct(v1, v2);
			Point p1((tr1.A.X + tr1.B.X + tr1.C.X) / 3, (tr1.A.Y + tr1.B.Y + tr1.C.Y) / 3, (tr1.A.Z + tr1.B.Z + tr1.C.Z) / 3);
			Point p2(p1.X + normal.A, p1.Y + normal.B, p1.Z + normal.C);
			vector<Point> pps;
			for (int j = 0; j < triangles_number_a; j++)
			{
				Point intersect_point;
				Triangle tr2 = model_a.GetTriangleVertices(j);
				Plane plane = tr2.GetPlane();
				if (plane.GetIntersectionWithLine(p1, p2, intersect_point) &&
					Vector3::DotProduct(Vector3(intersect_point, p1), Vector3(p2, p1)) > 0 &&
					tr2.IsPointInsideTriangle(intersect_point))
				{
					pps.push_back(intersect_point);
				}
			}

			for (int j = 0; j < pps.size(); j++)
			{
				for (int k = j + 1; k < pps.size(); k++)
				{
					if (pps[j] == pps[k])
					{
						pps.erase(pps.begin() + k);
						k--;
					}
				}
			}



			if (pps.size() % 2 != 0)
			{
				differ2.AddTriangle(tr1);
				index_d.push_back(i);
			}
			key = 0;
		}
		sort(index_d.begin(), index_d.end());
		for (int i = index_d.size() - 1; i > -1; i--)
			model_d.DeleteTriangle(index_d[i]);

		polymodels.push_back(model_c);
		polymodels.push_back(model_d);
		polymodels.push_back(differ1);
		polymodels.push_back(differ2);
		return polymodels;
	}

	void Intersection::DeletePolygons(PolyModel& model_a, PolyModel& model_b, vector<Polygon>&polygons_a, vector<Polygon>&polygons_b)
	{
		vector<int> tr_indexes_model_a;
		for (int i = 0; i < polygons_a.size(); i++)
			tr_indexes_model_a.push_back(polygons_a[i].triangle_index);
		vector<int> tr_indexes_model_b;
		for (int i = 0; i < polygons_b.size(); i++)
			tr_indexes_model_b.push_back(polygons_b[i].triangle_index);

		sort(tr_indexes_model_a.begin(), tr_indexes_model_a.end());
		sort(tr_indexes_model_b.begin(), tr_indexes_model_b.end());

		for (int i = tr_indexes_model_a.size() - 1; i > -1; i--)
			model_a.DeleteTriangle(tr_indexes_model_a[i]);

		for (int i = tr_indexes_model_b.size() - 1; i > -1; i--)
			model_b.DeleteTriangle(tr_indexes_model_b[i]);
	}

	void Intersection::AddPolygonsToModel(PolyModel&model_a, vector<Polygon>&polygons_a)
	{
		for (int i = 0; i < polygons_a.size(); i++)
			Triangle tr = model_a.GetTriangleVertices(polygons_a[i].triangle_index);
	}

	//ќбъедин€ет полигоны с одинаковым индексом треугольника, добавл€ет в полигоны точки треугольников,
	//удал€ет из полигонов одинаковые точки
	void Intersection::UnitePolygons(vector<Polygon>&polygons, PolyModel& model)
	{
		for (int i = 0; i < polygons.size(); i++)
		{
			for (int j = i + 1; j < polygons.size(); j++)
			{
				if (polygons[i].triangle_index == polygons[j].triangle_index)
				{
					for (int k = 0; k < polygons[j].points.size(); k++)
						polygons[i].points.push_back(polygons[j].points[k]);
					polygons.erase(polygons.begin() + j);
					j--;
				}
			}
		}

		for (int i = 0; i < polygons.size(); i++)
		{
			Triangle tr = model.GetTriangleVertices(polygons[i].triangle_index);
			polygons[i].points.insert(polygons[i].points.begin(), tr.C);
			polygons[i].points.insert(polygons[i].points.begin(), tr.B);
			polygons[i].points.insert(polygons[i].points.begin(), tr.A);
		}
		for (int i = 0; i < polygons.size(); i++)
		{
			for (int j = 0; j < polygons[i].points.size(); j++)
			{
				for (int k = j + 1; k < polygons[i].points.size(); k++)
				{
					if (polygons[i].points[j] == polygons[i].points[k])
					{
						polygons[i].points.erase(polygons[i].points.begin() + k);
						k--;
					}
				}
			}
		}
	}

	PolyModel Intersection::UnitePolymodels(PolyModel& model_a, PolyModel& model_b)
	{
		for (int i = 0; i < model_b.GetTrianglesCount(); i++)
			model_a.AddTriangle(model_b.GetTriangleVertices(i));
		return model_a;
	}
	PolyModel Intersection::RotateModelNormal(PolyModel & model)
	{
		PolyModel pm;
		for (int i = 0; i < model.GetTrianglesCount(); i++)
		{
			Triangle tr = model.GetTriangleVertices(i);
			pm.AddTriangle(tr.B, tr.A, tr.C);
		}
		return pm;
	}
}