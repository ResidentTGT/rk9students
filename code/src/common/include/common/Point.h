#pragma once

#include "common/common_api.h"

namespace rk9
{

	struct COMMON_API Point {
		double X, Y, Z;

		Point() :
			X(0.0),
			Y(0.0),
			Z(0.0)
		{}

		Point(double x, double y, double z) :
			X(x),
			Y(y),
			Z(z)
		{}

		Point(const Point & p) {
			X = p.X;
			Y = p.Y;
			Z = p.Z;
		}

		Point& operator=(const Point& other) {
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			return *this;
		}

		~Point() {}

		bool operator==(const rk9::Point& p) const
		{			
			return (X == p.X && Y == p.Y && Z == p.Z);			
		}
	};


}