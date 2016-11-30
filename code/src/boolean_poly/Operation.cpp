#include "Operation.h"
#include <algorithm>
#include "Intersection.h"
using namespace std;
namespace rk9
{
	PolyModel Operation::Add(PolyModel model_a, PolyModel model_b)
	{
		vector<Polygon> polygons_a, polygons_b;
		//������� ����������� ������� a � b � ���������� ���������� ����������� � �������� (polygons_a,polygons_b).
		//���� ������� �������� � ���� ����� ����������� ���� ������������� � ������ ������������ ����� �� �������(a ��� b).
		Intersection::IntersectModels(model_a, model_b, polygons_a, polygons_b);
		//������� �������������� ������������ �� �������
		Intersection::DeletePolygons(model_a, model_b, polygons_a, polygons_b);

		/*PolyModel g;
		g.AddTriangle(polygons_a[4].points[0], polygons_a[4].points[1], polygons_a[4].points[2]);
		g.AddTriangle(polygons_a[4].points[3], polygons_a[4].points[4], polygons_a[4].points[5]);
		g.AddTriangle(polygons_a[4].points[6], polygons_a[4].points[7], polygons_a[4].points[8]);
		g.WriteToSTLFile("A:/dev/rk9students/doc/Romanov/TestingModels/a.stl");*/
			
		//������������ ���������
		//Triangulate(polygons_a);
		//Triangulate(polygons_b);
		//���������� ������������������� ��������� � ������
		//AddPolygonsToModel(model_a, polygons_a);
		//AddPolygonsToModel(model_b, polygons_b);
		//��������� ������� �� 4 �����: 2 ������� ����������� � 2 ���������� ����� �������
		vector<PolyModel> polymodels = Intersection::DivideModels(model_a, model_b);
		PolyModel result = Intersection::UnitePolymodels(polymodels[0], polymodels[1]);
		return result;
	}

	PolyModel Operation::Intersect(PolyModel model_a, PolyModel model_b)
	{
		vector<Polygon> polygons_a, polygons_b;		
		Intersection::IntersectModels(model_a, model_b, polygons_a, polygons_b);
		Intersection::DeletePolygons(model_a, model_b, polygons_a, polygons_b);
		//Triangulate(polygons_a);
		//Triangulate(polygons_b);		
		//AddPolygonsToModel(model_a, polygons_a);
		//AddPolygonsToModel(model_b, polygons_b);		
		vector<PolyModel> polymodels = Intersection::DivideModels(model_a, model_b);
		PolyModel result = Intersection::UnitePolymodels(polymodels[2], polymodels[3]);
		return result;
	}

	PolyModel Operation::Substract(PolyModel model_a, PolyModel model_b)
	{
		vector<Polygon> polygons_a, polygons_b;
		Intersection::IntersectModels(model_a, model_b, polygons_a, polygons_b);
		Intersection::DeletePolygons(model_a, model_b, polygons_a, polygons_b);
		//Triangulate(polygons_a);
		//Triangulate(polygons_b);		
		//AddPolygonsToModel(model_a, polygons_a);
		//AddPolygonsToModel(model_b, polygons_b);		
		vector<PolyModel> polymodels = Intersection::DivideModels(model_a, model_b);
		PolyModel result = Intersection::UnitePolymodels(polymodels[0], polymodels[3]);
		return result;
	}	
}
