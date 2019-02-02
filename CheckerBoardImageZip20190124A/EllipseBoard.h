#pragma once
#include "CheckerBoard.h"

class EllipseBoard :
	public CheckerBoard
{
	//friend class CCheckerBoardImageDlg;
public:
	EllipseBoard();
	EllipseBoard(int width, int height, int columns, int rows, boolean randomSquare = false);
	~EllipseBoard();

	void Draw();
	void Fill(int row, int col, int wid, int hgt);
	void FillFullEllipse(int row, int col, int wid, int hgt);
	void FillQuarterEllipse(int row, int col, int wid, int hgt);
	void UL_Q(int row, int col, int wid, int hgt);
	void UR_Q(int row, int col, int wid, int hgt);
	void LL_Q(int row, int col, int wid, int hgt);
	void LR_Q(int row, int col, int wid, int hgt);
	void TopEdgeHalfEllipse(int row, int col, int wid, int hgt);
	void BtmEdgeHalfEllipse(int row, int col, int wid, int hgt);
	void LeftEdgeHalfEllipse(int row, int col, int wid, int hgt);
	void RightEdgeHalfEllipse(int row, int col, int wid, int hgt);
	//void LR_H(int row, int col, int wid, int hgt);
};

