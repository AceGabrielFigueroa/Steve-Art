#pragma once

#include "BooleanBitmap.h"

class CheckerBoard : public BooleanBitmap
{
public:
	CheckerBoard();
	CheckerBoard(int width, int height, int columns, int rows, boolean randomSquare=false);
	~CheckerBoard();
	CheckerBoard(CheckerBoard* chb1, CheckerBoard* chb2, boolean xor=false);
	//double drand(int n);
	//int irand(int n);
	void Construct(int width, int height, int cols, int rows);
	void Fill(int row, int col, int wid, int hgt);

	void Draw();
	//void ConvertToInt();
	static const int MAX_PIXELS_WIDTH = 1024; // must be multiple of 32
	static const int MAX_PIXELS_HEIGHT = 1024;

private:

protected:
	int m_columns;
	int m_rows;

	boolean m_random = false;


public:
	//UINT32 m_pixels[MAX_PIXELS_HEIGHT][MAX_PIXELS_WIDTH / sizeof(UINT32)];
	//boolean m_board[MAX_PIXELS_HEIGHT][MAX_PIXELS_WIDTH];

};

