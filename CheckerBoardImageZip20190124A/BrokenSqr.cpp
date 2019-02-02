#include "stdafx.h"
#include "BrokenSqr.h"


BrokenSqr::BrokenSqr()
{
}

BrokenSqr::BrokenSqr(int width, int height, int columns, int rows, boolean randomSquare)
{
	Construct(width, height, columns, rows);
	//Construct(width, height);
	//m_columns = columns;
	//m_rows = rows;
	m_random = randomSquare;
	Draw();
}


BrokenSqr::~BrokenSqr()
{
}

void BrokenSqr::Draw()
{
	//char buffer[100];
	//sprintf_s(buffer, "Draw Rows %2d  Cols %2d\n", 	m_rows, m_columns);
	//OutputDebugStringA(buffer);
	//int pixRow = 0;
	//int pixCol = 0;
	int blkWidth = m_widthPixels / m_columns;
	int blkHeight = m_heightPixels / m_rows;

	for (auto row = 0; row < m_rows; row++) {
		for (auto col = 0; col < m_columns; col++)
		{
			//int r = row * blkHeight;
			//int c = col * blkWidth;
			//int w = blkWidth;
			//int h = blkHeight;
			//Fill(r, c, blkWidth, blkHeight);
			Fill(row, col, blkWidth, blkHeight);
		}
	}

	//for (auto row = 0; row < m_rows; row++) {
	//	for (auto col = 0; col < m_columns; col++)
	//	{
	//		Fill(row, col, blkWidth, blkHeight);
	//	}
	//}
}

//void BrokenSqr::Fill(int row, int col, int wid, int hgt)
//{
//	// Choose two sides to draw a line between.
//	// Then choose a point on each of those sides
//	// to define the line.
//	// All points on one side of the line are 0
//	// and 1 on the other
//	int top = row;
//	int btm = row + hgt - 1;
//	int lft = col;
//	int rgt = col + wid - 1;
//
//	int x1, x2, y1, y2;	// end point coordinates
//	
//	// Choose the first side and get pt coordinates
//	int firstSide = GetPoint(x1, y1, row, col, wid, hgt);
//	
//	// Second point must be on a different side and 
//	// x coords must be different so we don't have a vertical line and
//	// y coords must be different so we don't have a horizontal line
//	int secondSide;
//	do { secondSide = GetPoint( x2, y2, row, col, wid, hgt); }
//	while ((firstSide == secondSide) || (x1==x2) || (y1==y2));
//
//	boolean pixelVal = irand(2) == 1;
//	double slope = ((double)(y2 - y1)) / (x2 - x1);
//	double intercept = -slope * x1 + y1;
//	for (auto rowY = top; rowY <= btm; rowY++)
//	{
//		for (auto colX = lft; colX <= rgt; colX++)
//		{
//			SetAt(rowY, colX, pixelVal ^ AboveLine(colX, rowY, slope, intercept));
//		}
//	}
//}

void BrokenSqr::Fill(int row, int col, int wid, int hgt)
{
	int r = row * hgt;
	int c = col * wid;
	int w = wid;
	int h = hgt;

	// Choose two sides to draw a line between.
	// Then choose a point on each of those sides
	// to define the line.
	// All points on one side of the line are 0
	// and 1 on the other
	int top = r;
	int btm = r + hgt - 1;
	int lft = c;
	int rgt = c + wid - 1;

	int x1, x2, y1, y2;	// end point coordinates

	// Choose the first side and get pt coordinates
	int firstSide = GetPoint(x1, y1, r, c, wid, hgt);

	// Second point must be on a different side and 
	// x coords must be different so we don't have a vertical line and
	// y coords must be different so we don't have a horizontal line
	int secondSide;
	do { secondSide = GetPoint(x2, y2, r, c, wid, hgt); } while ((firstSide == secondSide) || (x1 == x2) || (y1 == y2));

	boolean pixelVal = irand(2) == 1;
	double slope = ((double)(y2 - y1)) / (x2 - x1);
	double intercept = -slope * x1 + y1;
	for (auto rowY = top; rowY <= btm; rowY++)
	{
		for (auto colX = lft; colX <= rgt; colX++)
		{
			SetAt(rowY, colX, pixelVal ^ AboveLine(colX, rowY, slope, intercept));
		}
	}
}

int BrokenSqr::GetPoint(int &x, int &y, int row, int col, int wid, int hgt)
{
	int top = row;
	int btm = row + hgt - 1;
	int lft = col;
	int rgt = col + wid - 1;

	int side = irand(4);
	switch (side)
	{
	case LEFT:
		x = lft;
		y = top + irand(hgt);
		break;
	case RIGHT:
		x = rgt;
		y = top + irand(hgt);
		break;
	case TOP:
		x = lft + irand(wid);
		y= top;
		break;
	case BOTTOM:
		x = lft + irand(wid);
		y = btm;
		break;
	default:
		ASSERT(false);
	}
	return side;
}


inline boolean BrokenSqr::AboveLine(int x, int y, double slope, double intercept)
{
	double actualY = slope * x + intercept;
	return y < actualY;
}


