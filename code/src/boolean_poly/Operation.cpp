#include "Operation.h"
#include <algorithm>
#include "Intersection.h"
#include "TrianglesBuilder.h"
using namespace std;

namespace rk9
{
	

	PolyModel Operation::Add(PolyModel model_a, PolyModel model_b)
	{
		
		vector<Polygon> polygons_a, polygons_b;
		//Находит пересечение моделей a и b и записывает результаты пересечения в полигоны (polygons_a,polygons_b).
		//Один полигон включает в себя точки пересечения двух треугольников и индекс треугольника одной из моделей(a или b).
		Intersection::IntersectModels(model_a, model_b, polygons_a, polygons_b);
		//Удаляет пересекающиеся треугольники из моделей
		Intersection::DeletePolygons(model_a, model_b, polygons_a, polygons_b);					
		//Триангуляция полигонов		
		vector<Triangle> triangulation_a;
		TrianglesBuilder::buildIntersectionArea(triangulation_a, polygons_a);
		vector<Triangle> triangulation_b;
		TrianglesBuilder::buildIntersectionArea(triangulation_b, polygons_b);	
		//Добавление триангулированных треугольников в модели
		model_a.AddTriangles(triangulation_a);
		model_b.AddTriangles(triangulation_b);
		//Разбиение моделей на 4 части: 2 участка пересечения и 2 оставшиеся части моделей
		vector<PolyModel> polymodels = Intersection::DivideModels(model_a, model_b);	
		
		PolyModel result = Intersection::UnitePolymodels(polymodels[0], polymodels[1]);
		return result;
	}

	PolyModel Operation::Intersect(PolyModel model_a, PolyModel model_b)
	{
		vector<Polygon> polygons_a, polygons_b;		
		Intersection::IntersectModels(model_a, model_b, polygons_a, polygons_b);
		Intersection::DeletePolygons(model_a, model_b, polygons_a, polygons_b);
		vector<Triangle> triangulation_a;
		TrianglesBuilder::buildIntersectionArea(triangulation_a, polygons_a);
		vector<Triangle> triangulation_b;
		TrianglesBuilder::buildIntersectionArea(triangulation_b, polygons_b);
		model_a.AddTriangles(triangulation_a);
		model_b.AddTriangles(triangulation_b);
		vector<PolyModel> polymodels = Intersection::DivideModels(model_a, model_b);
		PolyModel result = Intersection::UnitePolymodels(polymodels[2], polymodels[3]);
		return result;
	}

	PolyModel Operation::Substract(PolyModel model_a, PolyModel model_b)
	{
		vector<Polygon> polygons_a, polygons_b;
		Intersection::IntersectModels(model_a, model_b, polygons_a, polygons_b);
		Intersection::DeletePolygons(model_a, model_b, polygons_a, polygons_b);
		vector<Triangle> triangulation_a;
		TrianglesBuilder::buildIntersectionArea(triangulation_a, polygons_a);
		vector<Triangle> triangulation_b;
		TrianglesBuilder::buildIntersectionArea(triangulation_b, polygons_b);
		model_a.AddTriangles(triangulation_a);
		model_b.AddTriangles(triangulation_b);
		vector<PolyModel> polymodels = Intersection::DivideModels(model_a, model_b);
		polymodels[3]=Intersection::RotateModelNormal(polymodels[3]);
		PolyModel result = Intersection::UnitePolymodels(polymodels[0], polymodels[3]);
		return result;
	}	
}
