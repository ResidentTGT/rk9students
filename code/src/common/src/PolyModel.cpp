#include <fstream>

#include "common/PolyModel.h"

using namespace std;

namespace rk9
{
	int PolyModel::ReadFromSTLFile(char * filename) {

		ifstream fin(filename, ios_base::binary);
		if (!fin) {
			return 1;
		}
		
		// Проверка, что файл бинарный
		char buf[6];
		buf[5] = '\0';
		fin.read(buf, 5);
		if (string(buf) == "solid") {
			return 1;
		}

		// Пропускаем заголовок длиной 80 байт
		fin.seekg(80, fin.beg);

		unsigned triangles_count;
		fin.read( reinterpret_cast<char *>(&triangles_count), sizeof(unsigned));

		float coordinates[9];

		// Заранее выделим место под точки и треугольники
		Triangles.reserve(triangles_count);

		for (unsigned i = 0; i < triangles_count; ++i) {
			// Пропускаем нормаль (3 x 4 байт, float)
			fin.seekg(3 * sizeof (float), fin.cur);
			
			// Считываем координаты трех вершин треугольника (по 3 координаты на вершину)
			fin.read(reinterpret_cast<char *>(coordinates), 9 * sizeof(float));

			// Пропускаем 16 бит "Attribute Byte Count"
			fin.seekg(sizeof(unsigned short), fin.cur);

			Point pt1, pt2, pt3;
			pt1.X = coordinates[0];
			pt1.Y = coordinates[1];
			pt1.Z = coordinates[2];

			pt2.X = coordinates[3];
			pt2.Y = coordinates[4];
			pt2.Z = coordinates[5];

			pt3.X = coordinates[6];
			pt3.Y = coordinates[7];
			pt3.Z = coordinates[8];

			AddTriangle(pt1, pt2, pt3);
		}

		return 0;
	}

	int PolyModel::WriteToSTLFile(char * filename) {
		
		ofstream fout(filename, ios_base::binary);
		if (!fout) {
			return 1;
		}

		// Запишем заголовок 80 байт
		char header[80];
		fout.write(header, 80);

		// Запишем кол-во треугольников
		unsigned triangles_count = Triangles.size();
		fout.write(reinterpret_cast<char *>(&triangles_count), sizeof(unsigned));

		// Запись треугольников
		for (unsigned i = 0; i < triangles_count; ++i) {

			// Запишем "нормаль" 3 х 4 байт
			float normal[3] = { 0.0, 0.0, 0.0 };
			fout.write(reinterpret_cast<char *>(normal), 12);

			// Запишем координаты треугольника
			float coordinates[9];
			Triangle tr = Triangles[i];
			
			
			
			coordinates[0] = static_cast<float>(tr.A.X);
			coordinates[1] = static_cast<float>(tr.A.Y);
			coordinates[2] = static_cast<float>(tr.A.Z);
						   	 
			coordinates[3] = static_cast<float>(tr.B.X);
			coordinates[4] = static_cast<float>(tr.B.Y);
			coordinates[5] = static_cast<float>(tr.B.Z);
						   	 						 
			coordinates[6] = static_cast<float>(tr.C.X);
			coordinates[7] = static_cast<float>(tr.C.Y);
			coordinates[8] = static_cast<float>(tr.C.Z);

			fout.write(reinterpret_cast<char *>(coordinates), 9 * sizeof(float));

			// Запишем "Attribute byte count" (16 бит)
			unsigned short attr_byte_count;
			fout.write(reinterpret_cast<char *>(&attr_byte_count), sizeof(unsigned short));

		}

		return 0;
	}

	void PolyModel::AddTriangle(Point p1, Point p2, Point p3)
	{
		Triangle t{ p1, p2, p3 };
		Triangles.push_back(t);
	}

	void PolyModel::AddTriangle(Triangle t)
	{
		Triangles.emplace_back(t);
	}
	
	//return the number of triangles in polymodel
	unsigned PolyModel::GetTrianglesCount() {
		return Triangles.size();
	}

	// Возвращает массив вершин треугольника под индексом index
	// Если треугольника с таким индексом нет, возвращает пустой массив
	Triangle PolyModel::GetTriangleVertices(unsigned index) {
		if (index <= GetTrianglesCount()) 
			return Triangles[index];
	}

	PolyModel* PolyModel::MergeModels(PolyModel & m1, PolyModel & m2)
	{
		PolyModel* result = new PolyModel();

		for (size_t i = 0; i < m1.GetTrianglesCount(); ++i)
			result->AddTriangle(m1.GetTriangleVertices(i));

		for (size_t i = 0; i < m2.GetTrianglesCount(); ++i)
			result->AddTriangle(m2.GetTriangleVertices(i));

		return result;
	}

}

