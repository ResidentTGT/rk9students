#include <iostream>
#include "common/PolyModel.h"

using namespace rk9;

int main(int argc, char* argv[])
{
	
	PolyModel model_a, model_b;

	// A simple usage example:
	// Read a model from file,
	// Add one more triangle,
	// then write the model 
	// to another file.

	model_a.ReadFromSTLFile(argv[1]);

	model_b.ReadFromSTLFile(argv[1]);

	PolyModel* res = PolyModel::MergeModels(model_a, model_b);

	res->WriteToSTLFile("output.stl");

	return 0;
}
