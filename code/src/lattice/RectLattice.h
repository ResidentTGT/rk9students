#pragma once
#include<vector>
#include "PointWithDensity.h"
#include "fparser.h"

using namespace std;

namespace rk9 {

	struct RectLattice {

		//координаты вершин ограничивающего параллелепипеда, размеры в мм

		double
			Xmin = -100, 
			Xmax = +100,
			Ymin = -100,
			Ymax = +100,
			Zmin = -100,
			Zmax = +100;

		//создание прямоугольной решетки, pace - шаг решетки
		//возвращает вектор точек, лежащих в узлах решетки
		vector<PointWithDensity> GenerateRectLattice(double pace);

		//Определение значения плотности в узлах решетки, nodes - вектор узлов
		vector <PointWithDensity> RectLattice::DefineNodeDensity(string function, vector<PointWithDensity>  nodes);
	};


	//функция, которая вычисляет значение функции в точке
	// function - функция, заданная неявно,
	// p - точка, в которой необходимо вычислить значение
	float CalculateFuncValue(string function, PointWithDensity p);

	//Определяем, находится ли точка внутри объема, заданного неяной функцией
	bool IsPointIntoVolum(string function, PointWithDensity p);
}

