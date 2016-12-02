#include "TrianglesBuilder.h"
#include "common/Point.h"
#include "Polygon.h"
using namespace std;

namespace rk9 {
	TrianglesBuilder::TrianglesBuilder() {}

	void  TrianglesBuilder::buildIntersectionArea(vector<Triangle> &triangulation, vector<Polygon> &ps) {
		TrianglesBuilder builder;
		for (int i = 0; i < ps.size(); i++)
		{
			vector<Point> vec = ps[i].points;
			builder.buildTriangles(triangulation, vec);
		}
	}

	void TrianglesBuilder::updateFrontier(
		std::set<Edge> &frontier, Point &a, Point &b) {
		Edge e(a, b);
		//проверяем есть ли ребро в сете
		auto it = frontier.find(e);
		if (it != frontier.end()) {
			frontier.erase(it);
		}
		else {
			e.flip(normal_, d_);
			frontier.insert(e);
		}
	}
	
	Edge TrianglesBuilder::hullEdge(std::vector<Point> &s, int n) {
		int m = 0;
		int i;
		for (i = 1; i < n; i++)
			if (s[i] < s[m])
				m = i;
		Point temp = s[0];
		s[0] = s[m];
		s[m] = temp;
		for (m = 1, i = 2; i < n; i++) {
			int c = s[i].classify(s[0], s[m]);
			if ((c == Point::LEFT) || (c == Point::BETWEEN))
				m = i;
		}
		return Edge(s[0], s[m]);
	}

	bool TrianglesBuilder::mate(Edge &e, std::vector<Point> &s, int n, Point &p) {
		Point *bestp = NULL;
		double t = 0.f, bestt = FLT_MAX;
		Edge f = e;
		//f - перпендикуляр в середине отрезка "e"
		f.rot(normal_, d_);
		for (int i = 0; i < n; i++) {
			int c = s[i].classify(e.org, e.dest);
			if (c == Point::RIGHT) {
				Edge g(e.dest, s[i]);
				g.rot(normal_, d_);
				f.intersect(g, t, normal_, d_);
				if (t < bestt) {
					bestp = &s[i];
					bestt = t;
				}
			}
		}
		if (bestp) {
			p = *bestp;
			return true;
		}
		return false;
	}


	Point TrianglesBuilder::getNormal(vector<Point> &s, double &orientation) {
		Point normal;
		int n = s.size();
		for (int i = 0; i < n-2; i++) {
			Point v1 = s[i] - s[i+1];
			Point v2 = s[i+1] - s[i+2];
			normal = v1.crossProduct(v2);
			// алгоритм только для правой тройки векторов
			// orientation 
			orientation = Point::mixedProduct(Point(1,0,0), Point(0,1,0), normal);
			if (!normal.isNullVec()) {
				normal.normilize();
				return normal;
			}
		}
		assert(false && "All points are collinear!");
	}

	/*для упращения читаемости кода*/
	Point TrianglesBuilder::getNormal(vector<Point> &s) {
		double o;
		return getNormal(s, o);
	}

	void TrianglesBuilder::checkPoints(Point normal, double d, vector<Point> &s) {
		for (int i = 0; i < s.size(); i++) {
			assert(s[i].isInPlane(normal, d), "Points are not co-plannar!");
		}
	}

	vector<Point> TrianglesBuilder::swapXZ(vector<Point> &s) {
		vector<Point> res;
		for (Point p : s) {
			res.push_back(Point(p.Z, p.Y, p.X));
		}
		return res;
	}

	Point TrianglesBuilder::swapXZ(Point &p) {
		return Point(p.Z, p.Y, p.X);
	}

	vector<Point> TrianglesBuilder::swapYZ(vector<Point> &s) {
		vector<Point> res;
		for (Point p : s) {
			res.push_back(Point(p.X, p.Z, p.Y));
		}
		return res;
	}

	Point TrianglesBuilder::swapYZ(Point &p) {
		return Point(p.X, p.Z, p.Y);
	}

	vector<Point> TrianglesBuilder::swapXY(vector<Point> &s) {
		vector<Point> res;
		for (Point p : s) {
			res.push_back(Point(p.Y, p.X, p.Z));
		}
		return res;
	}

	Point TrianglesBuilder::swapXY(Point &p) {
		return Point(p.Y, p.X, p.Z);
	}

	// A*x + B*y + C*z = D
	// наличие индекса в типе нормали означает, что этот индекс не равен нулю
	enum
	{
		NORMAL_ABC,
		NORMAL_AB,
		NORMAL_AC,
		NORMAL_BC,
		NORMAL_A,
		NORMAL_B,
		NORMAL_C
	};

	int TrianglesBuilder::classifyNormal(Point n) {
		if (n.X != 0 && n.Y != 0 && n.Z != 0) return NORMAL_ABC;
		if (n.X != 0 && n.Y != 0) return NORMAL_AB;
		if (n.X != 0 && n.Z != 0) return NORMAL_AC;
		if (n.Y != 0 && n.Z != 0) return NORMAL_BC;
		if (n.X != 0) return NORMAL_A;
		if (n.Y != 0) return NORMAL_B;
		if (n.Z != 0) return NORMAL_C;
		assert(!n.isNullVec(), "Normal cannot be zero vector!");
	}

	void TrianglesBuilder::modifyPoints(
		vector<Point> &s, 
		int normal_type,
		double &orientation
	) {
		//по умолчанию алгоритм работает со свободными координатами x y
		//в некоторых случаях необходимо поменять координаты местами, чтобы свести задачу к уже решенной
		switch (normal_type)
		{
			case NORMAL_ABC:
			{
				//свободные x y
				//зависимая z
				return;
			}
			case NORMAL_AB:
			{
				//свободные x z
				//зависимая y
				s = swapYZ(s);
				// сохранение ориентации
				s = swapXY(s);
				// после изменеия ориентации изменились 
				// нормаль и Д
				normal_ = getNormal(s, orientation);
				d_ = normal_.dotProduct3D(s[0]);
				return;
			}
			case NORMAL_AC:
			{
				//свободные x y
				//зависимая z
				return;
			}
			case NORMAL_BC:
			{
				//свободные x y
				//зависимая z
				return;
			}
			case NORMAL_A:
			{
				//свободные y z
				//зависимая x
				s = swapXZ(s);
				s = swapXY(s);
				normal_ = getNormal(s, orientation);
				d_ = normal_.dotProduct3D(s[0]);
				return;
			}
			case NORMAL_B:
			{
				//свободные x z
				//зависимая y
				s = swapYZ(s);
				s = swapXY(s);
				normal_ = getNormal(s, orientation);
				d_ = normal_.dotProduct3D(s[0]);
				break;
			}
			case NORMAL_C:
			{
				//свободные x y
				//зависимая z
				break;
			}
		};
	}

	Point TrianglesBuilder::InvPointTransform(Point p, int normal_type) {
		switch (normal_type)
		{
			case NORMAL_ABC:
			{
				return p;
			}
			case NORMAL_AB:
			{
				return swapYZ(swapXY(p));
			}
			case NORMAL_AC:
			{
				return p;
			}
			case NORMAL_BC:
			{
				return p;
			}
			case NORMAL_A:
			{
				return swapXZ(swapXY(p));
			}
			case NORMAL_B:
			{
				return swapYZ(swapXY(p));
			}
			case NORMAL_C:
			{
				return p;
			}
		};
	}

	/*
	void TrianglesBuilder::deleteCollinear(vector<Point> &s) {
		int size = s.size();
		int i, j, k;
		set<Point> toDelete;
		for (i = 0; i < size; i++) {
			for (j = 0; j < size; j++) {
				for (k = 0; k < size; k++) {
					if (i == j || i == k || j == k) continue;
					Point v1 = s[i] - s[k];
					Point v2 = s[j] - s[k];
					Point cD = v1.crossProduct(v2);
					if (cD.isNullVec()) {
						int c = s[k].classify(s[i],s[j]);
						if (c == Point::BETWEEN)
							toDelete.insert(s[k]);
						if (c == Point::BEYOND)
							toDelete.insert(s[j]);
						if (c == Point::BEHIND)
							toDelete.insert(s[i]);
					}
				}
			}
		}
		for (Point p : toDelete) s.erase(std::find(s.begin(), s.end(), p));
	}
	*/

	void TrianglesBuilder::buildTriangles(vector<Triangle> &triangles, vector<Point> s) {
		//triangles.clear();
		//deleteCollinear(s);

		assert(s.size() >= 3, "Not enough points!");
		Point p(0,0,0);
		//ориентация тройки векторов должна быть правая
		//интовая переменная orientation передается в функции по ссылке!
		// ориентация полигона кот подали
		double orientation;
		normal_ = getNormal(s, orientation);
		// получаем D плоскости
		d_ = normal_.dotProduct3D(s[0]);
		// проверяем, лежат ли все точки s 
		// в одной плоскости
		checkPoints(normal_, d_, s);
		// классификация нормали
		int normal_type = classifyNormal(normal_);

		// путем отражения сводим задачу к уже решенной
		modifyPoints(s, normal_type, orientation);
		// если левая тройка векторов
		// меняем на правую
		// rotate вращает по часовой 
		if (orientation < 0) {
			normal_ = -1 * normal_;
			d_ = normal_.dotProduct3D(s[0]);
		}

		checkPoints(normal_, d_, s);
		
		int n = s.size();
		Edge e = hullEdge(s, n);
		std::set<Edge> frontier;
		frontier.insert(e);
		while (!frontier.empty()) {
			//std::cout << "size: " << frontier.size() << std::endl;
			e = *frontier.rbegin(); 
			frontier.erase(e);
			if (mate(e, s, n, p)) {
				updateFrontier(frontier, p, e.org);
				updateFrontier(frontier, e.dest, p);
				Triangle tr;

				if (orientation < 0)
					tr = Triangle(InvPointTransform(e.org, normal_type),
								  InvPointTransform(e.dest, normal_type),
								  InvPointTransform(p, normal_type));
				else
					tr = Triangle(InvPointTransform(e.dest, normal_type),
								  InvPointTransform(e.org, normal_type),
						          InvPointTransform(p, normal_type));
				triangles.insert(triangles.begin(), tr);
			}
		}
	}
}