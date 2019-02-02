#pragma once
#include "CheckerBoard.h"
class Quadrilateral :
	public CheckerBoard
{
public:
	Quadrilateral();
	Quadrilateral(int width, int height, int columns, int rows, boolean randomSquare = false);

	~Quadrilateral();
	void Draw();
	void Fill(int row, int col, int wid, int hgt);
	int  xValue(int x1, int y1, int x2, int y2, int y);
	void FillLine(int xleft, int xright, int wid, int row, int col, boolean outer);
};

