#include <limits>

#include "common/Triangle.h"


using namespace std;

namespace rk9
{
	Vector3 Triangle::GetNormal() {
		Plane p = GetPlane();
		return Vector3{ p.A, p.B, p.C };
	};

	Plane Triangle::GetPlane() {
		//find coordinates of vectors of triangle's vertices
		Vector3 v01{ Verts[0], Verts[1] };
		Vector3 v02{ Verts[0], Verts[2] };

		Vector3 product = Vector3::CrossProduct(v01, v02);

		double A, B, C, D;
		A = product.A;
		B = product.B;
		C = product.C;
		D = -A * Verts[1].X - B * Verts[1].Y - C * Verts[1].Z;

		return Plane{ A, B, C, D };
	}

	// Вспомогательная функция для расчета длины гипотенузы (расстояния между точками)
	double hypot(Point a, Point b) {
		return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2) + pow(a.Z - b.Z, 2));
	}

	bool Triangle::IsPointInsideTriangle(Point p)
	{
		Triangle 
			t1{ A, B, p },
			t2{ A, C, p },
			t3{ B, C, p };

		double diff = t1.GetArea() + t2.GetArea() + t3.GetArea()
					  - this->GetArea();

		return diff <std::numeric_limits<float>::epsilon();
	};
}