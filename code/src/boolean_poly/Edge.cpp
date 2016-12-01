#include "Edge.h"
#include <cfloat>

using namespace std;

namespace rk9 {

	Edge::Edge(Point& _org, Point& _dest) : org(_org), dest(_dest)
	{
	}

	Edge::Edge(void) : org(Point(0, 0)), dest(Point(1, 0))
	{
	}


	bool cmpEdges(Edge a, Edge b) {
		if (a.org < b.org) return true;
		if (a.org > b.org) return false;
		if (a.dest < b.dest) return true; //?
		if (a.dest > b.dest) return false;
		return false;
	}

	bool Edge::operator<(Edge& p) {
		return cmpEdges(*this, p);
	}

	bool Edge::operator== (Edge& p) {
		return org == p.org && dest == p.dest;
	}

	bool Edge::operator!= (Edge& p) {
		return !(*this == p);
	}

	const bool Edge::operator<(const Edge& p) const {
		return cmpEdges(*this, p);
	}

	const bool Edge::operator== (const Edge& p) const {
		return org == p.org && dest == p.dest;
	}


	Edge& Edge::rot(Point normal, double d) {
		//поворот на 90 градусов
		Point m = (dest + org) / 2;
		//Point v = dest - org;
		Point temp = dest;
		// поворачиваем ребро
		dest = Point::Rotate(m, normal, 90, org);
		org = Point::Rotate(m, normal, 90, temp);
		//Point n = dest - org;
		//org.X*normal.X + org.Y*normal.Y + org.Z*normal.Z - d == 0
		//dest.X*normal.X + dest.Y*normal.Y + dest.Z*normal.Z - d == 0
		//double len_v = v.length();
		//double len_n = n.length();
		//double cD = v.dotProduct3D(n);
		assert(fabs(org.dotProduct3D(normal) - d) < Point::Tol);
		assert(fabs(dest.dotProduct3D(normal) - d) < Point::Tol);
		assert((dest + org) / 2 == m);
		return *this;
	}

	Edge& Edge::flip(Point n, double d) {
		//поворот на 180 градусов
		Point temp = org;
		org = dest;
		dest = temp;
		return *this;
		//return rot(n,d).rot(n,d);
	}


	int Edge::intersect(Edge& e, double& t, Point normal, double d_) {
		Point a = org;
		Point b = dest;
		Point c = e.org;
		Point d = e.dest;
		
		//Point n = Point((d - c).Y, (c - d).X);
		Edge f = e;
		f.rot(normal, d_);
		Point n = f.dest - f.org;
		double denom = n.dotProduct3D(b - a);
		if (fabs(denom) <= Point::Tol) {
			/*
			int aclass = org.classify(e.org, e.dest);
			if ((aclass == Point::LEFT) || (aclass == Point::RIGHT))
				return PARALLEL;
			else
				return COLLINEAR;
			*/
			t = FLT_MAX;
			return PARALLEL;
		}
		double num = n.dotProduct3D(a - c);
		t = -num / denom;
		return SKEW;
	}

	Edge::~Edge()
	{
	}
}