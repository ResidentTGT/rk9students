#include "RectLattice.h"
#include "PointWithDensity.h"
#include "fparser.h"

#include <vector>

using namespace std;

namespace rk9
{

	vector <PointWithDensity> RectLattice::GenerateRectLattice(double pace) {

		vector <PointWithDensity> points;
		PointWithDensity point;

		for (double Z = Zmin; Z < Zmax; Z += pace)
			for (double Y = Ymin; Y < Ymax; Y += pace)
				for (double X = Xmin; X < Xmax; X += pace) {
					point.X = X;
					point.Y = Y;
					point.Z = Z;

					points.push_back(point);
				}

		return points;
	}


	vector <PointWithDensity> RectLattice::DefineNodeDensity(string function, vector<PointWithDensity>  nodes) {

		vector <PointWithDensity> points;

		for (unsigned i = 0; i < nodes.size(); i++) {

			if (IsPointIntoVolum(function, nodes[i]) == true) {
				nodes[i].dens = 1;
				points.push_back(nodes[i]);
			}

			else {
				nodes[i].dens = 0;
				points.push_back(nodes[i]);
			}
		}

		return points;
	}
	

	float CalculateFuncValue(string function, PointWithDensity p) {

		FunctionParser parser;
		parser.Parse(function, "x,y,z");

		if (parser.EvalError())
		{
			printf("parse error\n");
			return false;
		}

		float coords[3]{ p.X, p.Y, p.Z };
		return (parser.Eval(coords));

	}


	bool IsPointIntoVolum(string function, PointWithDensity p) {

		float func_value = CalculateFuncValue(function, p);

		if (func_value > 0.0) {
			return false;
		}

		else return true;

	}
}