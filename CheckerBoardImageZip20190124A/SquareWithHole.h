#pragma once
#include "CheckerBoard.h"
class SquareWithHole :
	public CheckerBoard
{
public:
	SquareWithHole();
	~SquareWithHole();

	SquareWithHole(int width, int height, int columns, int rows );
	void Draw();
	void Fill(int row, int col, int wid, int hgt);
	//void CalculateHole(int wid, int hgt, int & r1, int & r2, int & c1, int & c2);
	void CalculateHole(int wid, int hgt, int *r1, int *r2, int *c1, int *c2);
};

