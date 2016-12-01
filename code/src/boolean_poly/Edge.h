#pragma once
#include "common/Point.h"

namespace rk9 {
	
	class Edge {
		public:
			Point org;
			Point dest;
			Edge(Point &org, Point &dest);
			Edge(void);
			~Edge();
			bool operator== (Edge&);
			bool operator!= (Edge&);
			bool operator< (Edge&);
			const bool operator< (const Edge&) const;
			const bool operator== (const Edge&) const;
			Edge& rot(Point,double);
			Edge& flip(Point,double);
			enum { COLLINEAR, PARALLEL, SKEW, SKEW_CROSS, SKEW_NO_CROSS };
			int intersect(Edge&, double&, Point normal, double d);
	};
}