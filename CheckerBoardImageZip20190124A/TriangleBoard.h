#pragma once
#include "CheckerBoard.h"
class TriangleBoard :
	public CheckerBoard
{
public:
	TriangleBoard();
	TriangleBoard(int width, int height, int columns, int rows, boolean randomSquare = false);

	~TriangleBoard();
	void Fill(int row, int col, int wid, int hgt);
	void ULTriangle(int row, int col, int wid, int hgt);
	void LRTriangle(int row, int col, int wid, int hgt);
	void URTriangle(int row, int col, int wid, int hgt);
	void LLTriangle(int row, int col, int wid, int hgt);
	void TopEdgeIsoceles(int row, int col, int wid, int hgt);
	void BtmEdgesoceles(int row, int col, int wid, int hgt);
	void LftEdgeIsoceles(int row, int col, int wid, int hgt);
	void RgtEdgeIsoceles(int row, int col, int wid, int hgt);

	void RandomTriangle(int row, int col, int wid, int hgt);

	void TopEdgeRandom(int row, int col, int wid, int hgt);
	void BtmEdgeRandom(int row, int col, int wid, int hgt);
	void LftEdgeRandom(int row, int col, int wid, int hgt);
	void RgtEdgeRandom(int row, int col, int wid, int hgt);
	void Draw();
};

