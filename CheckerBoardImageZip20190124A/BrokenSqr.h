#pragma once
#include "CheckerBoard.h"
class BrokenSqr :
	public CheckerBoard
{
public:
	BrokenSqr();
	BrokenSqr(int width, int height, int columns, int rows, boolean randomSquare = false);

	~BrokenSqr();
	void Fill(int row, int col, int wid, int hgt);
	int GetPoint(int &x, int &y, int row, int col, int wid, int hgt);
	boolean AboveLine(int x, int y, double slope, double intercept);

	enum SIDE { LEFT, RIGHT, TOP, BOTTOM };

	void Draw();
};

