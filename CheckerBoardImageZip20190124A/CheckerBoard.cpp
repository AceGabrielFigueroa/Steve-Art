#include "stdafx.h"
#include "CheckerBoard.h"
#include "BooleanBitmap.h"
#include <assert.h>


char bufr[200];
#define Ou OutputDebugStringA
#define CH(c) sprintf_s(bufr, "%c", c);
#define NL() CH('\n');

CheckerBoard::CheckerBoard()
{
}


CheckerBoard::~CheckerBoard()
{
}

CheckerBoard::CheckerBoard(int width, int height, int columns, int rows, boolean randomSquare)
{
	Construct(width, height, columns, rows);
	//m_columns = columns;
	//m_rows = rows;
	m_random = randomSquare;
	Draw();
}

void CheckerBoard::Draw()
{
	//char buffer[100];
	//sprintf_s(buffer, "Draw Rows %2d  Cols %2d\n", 	m_rows, m_columns);
	//OutputDebugStringA(buffer);
	//int pixRow = 0;
	//int pixCol = 0;

	//for (auto row = 0; row < m_rows; row++) {
	//	for (auto col = 0; col < m_columns; col++) 
	//	{
	//		boolean pixelValue;
	//		if (m_random)
	//			pixelValue = (rand() % 2) == 0;
	//		else	
	//			pixelValue = ((row + col) % 2) == 0 ? false : true;

	//		int blkWidth = m_widthPixels / m_columns;
	//		int blkHeight = m_heightPixels / m_rows;
	//		for (auto blkRow = 0; blkRow < blkHeight; blkRow++) {
	//			for (auto blkCol = 0; blkCol < blkWidth; blkCol++) {
	//				pixRow = row * blkHeight + blkRow;
	//				pixCol = col * blkWidth + blkCol;
	//				SetAt(pixRow, pixCol, pixelValue);
	//			}
	//		}
	//	}
	//}
	//Show();

	int blkWidth = m_widthPixels / m_columns;
	int blkHeight = m_heightPixels / m_rows;
	for (auto row = 0; row < m_rows; row++) {
		for (auto col = 0; col < m_columns; col++)
		{
			Fill(row, col, blkWidth, blkHeight);
		}
	}
}

void CheckerBoard::Fill(int row, int col, int wid, int hgt)
{
	boolean pixelValue;
	if (m_random)
		pixelValue = (rand() % 2) == 0;
	else
		pixelValue = ((row + col) % 2) == 0 ? false : true;

	int pixRow = 0;
	int pixCol = 0;

	int blkWidth = m_widthPixels / m_columns;
	int blkHeight = m_heightPixels / m_rows;
	for (auto blkRow = 0; blkRow < blkHeight; blkRow++) {
		for (auto blkCol = 0; blkCol < blkWidth; blkCol++) {
			pixRow = row * blkHeight + blkRow;
			pixCol = col * blkWidth + blkCol;
			SetAt(pixRow, pixCol, pixelValue);
		}
	}
}


void CheckerBoard::Construct(int width, int height, int cols, int rows)
{
	m_columns = cols;
	m_rows = rows;
	BooleanBitmap::Construct(width, height);
}


//// returns real (0.0, n) ie open interval.
//// 
//double  CheckerBoard::drand(int n)
//{
//	return (double)(rand() % n) / (double)n;
//}
//
//// returns int 0 to n-1
//int CheckerBoard::irand(int n)
//{
//	return rand() % n;
//}