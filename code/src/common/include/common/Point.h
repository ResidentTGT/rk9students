
#pragma once
#include "common/common_api.h"
#include <cmath>
#include <cassert>


namespace rk9
{
	class COMMON_API Point {
	private:
	public:
		double X, Y, Z;

		Point() :
			X(0.0),
			Y(0.0),
			Z(0.0)
		{}

		Point(double X, double Y, double z = 0) :
			X(X),
			Y(Y),
			Z(z)
		{}

		~Point() {}

		Point(const Point & p) {
			X = p.X;
			Y = p.Y;
			Z = p.Z;
		}
		static double Tol;
		inline static double RadFromDeg(double x)
		{
			return (3.141592653589793238463 / 180) * x;
		}

		inline static void SetTolerance(double t) {
			assert(Tol >= 0);
			Tol = t;
		}

		inline static Point Rotate(Point& PointOnAxis, Point& AxisDir, double Angle, Point& v)
		{
			assert(fabs(AxisDir.length() - 1) <= Tol, "AxisDir must be unit vector");
			Point r = v - PointOnAxis;
			// 1. делаем сдвиг PointOnAxis в начало координат
			// 2. совмещаем AxisDir с осью Z
			// поворачивает на Angle относ Z
			// обратное преобразование 2
			// обратное преобраз 1
			return PointOnAxis + cos(RadFromDeg(Angle)) * r
				+ ((1 - cos(RadFromDeg(Angle))) * AxisDir.dotProduct3D(r)) * AxisDir
				+ sin(RadFromDeg(Angle)) * AxisDir.crossProduct(r);
		}

		Point& operator=(const Point& other) {
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			return *this;
		}


		bool operator<(const rk9::Point& p) {
			return (X < p.X) && (fabs(X - p.X) > Tol) ||
				(fabs(X - p.X) <= Tol) && (Y < p.Y) && (fabs(Y - p.Y) > Tol) ||
				(fabs(X - p.X) <= Tol) && (fabs(Y - p.Y) <= Tol) && (Z < p.Z) && (fabs(Z - p.Z) > Tol);
		}

		Point operator- (Point& p) {
			return Point(X - p.X, Y - p.Y, Z - p.Z);
		}

		bool operator<(Point& p) {
			return (X < p.X) && (fabs(X - p.X) > Tol) ||
				(fabs(X - p.X) <= Tol) && (Y < p.Y) && (fabs(Y - p.Y) > Tol) ||
				(fabs(X - p.X) <= Tol) && (fabs(Y - p.Y) <= Tol) && (Z < p.Z) && (fabs(Z - p.Z) > Tol);
		}


		bool operator>(const rk9::Point& p) {
			return (X > p.X) && (fabs(X - p.X) > Tol) ||
				(fabs(X - p.X) <= Tol) && (Y > p.Y) && (fabs(Y - p.Y) > Tol) ||
				(fabs(X - p.X) <= Tol) && (fabs(Y - p.Y) <= Tol) && (Z > p.Z) && (fabs(Z - p.Z) > Tol);
		}


		Point Point::operator+ (Point& p) {
			return Point(X + p.X, Y + p.Y, Z + p.Z);
		}

		int Point::operator==(Point& p) {
			return fabs(X - p.X) <= Tol && fabs(Y - p.Y) <= Tol && fabs(Z - p.Z) <= Tol;
		}

		int Point::operator!=(Point& p) {
			return !(*this == p);
		}

		const int Point::operator<(const Point& p) const {
			return ((X < p.X) || ((fabs(X - p.X) <= Tol) && (Y < p.Y)));
		}

		const int Point::operator>(const Point& p) const {
			return ((X > p.X) || ((fabs(X - p.X) <= Tol) && (Y > p.Y)));
		}

		bool operator==(const rk9::Point& p) const
		{
			return fabs(X - p.X) <= Tol && fabs(Y - p.Y) <= Tol && fabs(Z - p.Z) <= Tol;
		}

		const int Point::operator!=(const Point& p) const {
			return !(*this == p);
		}

		friend Point operator* (double s, Point& p) {
			return Point(s*p.X, s*p.Y, s*p.Z);
		}

		friend Point operator/ (Point& p, double s) {
			assert(s != 0);
			return Point(p.X / s, p.Y / s, p.Z / s);
		}

		enum { LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION };

		int classify(Point&, Point&);

		double Point::length() const {
			return sqrt(X*X + Y*Y + Z*Z);
		}

		void Point::normilize() {
			double l = length();
			X = X / l;
			Y = Y / l;
			Z = Z / l;
		}

		double Point::dotProduct2D(Point& p) {
			return X * p.X + Y * p.Y;
		}

		double Point::dotProduct3D(Point& p) {
			return X * p.X + Y * p.Y + Z * p.Z;
		}

		Point Point::crossProduct(Point& p) {
			return Point(Y*p.Z - Z*p.Y, Z*p.X - X*p.Z, X*p.Y - Y*p.X);
		}

		double static Point::mixedProduct(Point& a, Point &b, Point& c) {
			return a.crossProduct(b).dotProduct3D(c);
		}


		bool Point::isNullVec() {
			return fabs(X) <= Tol && fabs(Y) <= Tol && fabs(Z) <= Tol;
		}

		bool Point::isInPlane(Point& normal, double d) {
			double zero = dotProduct3D(normal) - d;
			return fabs(zero) <= Tol;
		}

	};

}