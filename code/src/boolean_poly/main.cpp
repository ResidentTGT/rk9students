#include <vector> 

#include "common/PolyModel.h" 
#include "common/Point.h" 
#include "common/Triangle.h" 
#include "Polygon.h"
#include "common/Vector3.h"
#include <algorithm>
#include "Operation.h"
using namespace std;
using namespace rk9;

int main(int argc, char ** argv) {
	int key = 3;
	PolyModel model_a, model_b;

	model_a.ReadFromSTLFile("A:/dev/rk9students/doc/Romanov/TestingModels/11(ball).stl");
	model_b.ReadFromSTLFile("A:/dev/rk9students/doc/Romanov/TestingModels/12(ball).stl");

	PolyModel result;
	
	switch (key) {
	case 1:
		result = Operation::Add(model_a, model_b);
		break;
	case 2:
		result = Operation::Intersect(model_a, model_b);
		break;
	case 3:
		result = Operation::Substract(model_a, model_b);
		break;
	default:;
	}
	result.WriteToSTLFile(argv[3]);
	return 0;
}



