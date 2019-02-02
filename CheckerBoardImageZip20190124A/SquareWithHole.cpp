#include "stdafx.h"
#include "SquareWithHole.h"


SquareWithHole::SquareWithHole()
{
}


SquareWithHole::~SquareWithHole()
{
}

SquareWithHole::SquareWithHole(
	int width, int height, int columns, int rows)
{
	Construct(width, height, columns, rows);
	//Construct(width, height);
	//m_columns = columns;
	//m_rows = rows;
	m_random = true;
	Draw();
}


void SquareWithHole::Draw()
{
	int blkWidth = m_widthPixels / m_columns;
	int blkHeight = m_heightPixels / m_rows;
	for (auto row = 0; row < m_rows; row++) {
		for (auto col = 0; col < m_columns; col++)
		{
			Fill(row, col, blkWidth, blkHeight);
		}
	}
}
//Show();

void SquareWithHole::Fill(int row, int col, int wid, int hgt)
{
	boolean inner = (rand() % 2) == 0;
	boolean outer = !inner;

	int r1=0, r2=0, c1=0, c2=0;
	CalculateHole(wid, hgt, &r1, &r2, &c1, &c2);

	// Above Hole
	for (auto blkRow = 0; blkRow < r1; blkRow++)
	{
		int r = row * hgt + blkRow;
		for (auto blkCol = 0; blkCol < wid; blkCol++)
			SetAt(r, col * wid + blkCol, outer);
	}
	// In Hole Rows
	for (auto blkRow = r1; blkRow < r2; blkRow++)
	{
		// Before Hole
		int r = row * hgt + blkRow;
		for (auto blkCol = 0; blkCol < c1; blkCol++)
			SetAt(r, col * wid + blkCol, outer);
		// In Hole
		for (auto blkCol = c1; blkCol < c2; blkCol++)
			SetAt(r, col * wid + blkCol, inner);
		// After Hole
		for (auto blkCol = c2; blkCol < wid; blkCol++)
			SetAt(r, col * wid + blkCol, outer);
	}

	// Below
	for (auto blkRow = r2; blkRow < hgt; blkRow++)
	{
		int r = row * hgt + blkRow;
		for (auto blkCol = 0; blkCol < wid; blkCol++)
			SetAt(r, col * wid + blkCol, outer);
	}
}

void SquareWithHole::CalculateHole(int wid, int hgt, int *r1, int *r2, int *c1, int *c2)
{
	int blockArea = wid * hgt;
	double holeAmt;
	int _c1, _c2, _r1, _r2;
	//////// Get location of hole ///////////////
	int holeArea;
	do {
		_c1 = rand() % wid;
		assert(_c1 < wid);
		do { _c2 = rand() % wid; } while (_c1 == _c2);

		assert(_c2 < wid);
		if (_c2 < _c1)
		{
			int cx = _c1;
			_c1 = _c2;
			_c2 = cx;
		}

		_r1 = rand() % hgt;
		assert(_r1 < hgt);
		do { _r2 = rand() % hgt; } while (_r1 == _r2);
		assert(_r2 < hgt);
		if (_r2 < _r1)
		{
			int rx = _r1;
			_r1 = _r2;
			_r2 = rx;
		}

		holeArea = (_c2 - _c1) * (_r2 - _r1);
		holeAmt = ((double)holeArea) / blockArea;
	} while (holeAmt < 0.25 || holeAmt > 0.75);
	////////////  END - Calculate Hole ////////////////
	*c1 = _c1;
	*c2 = _c2;
	*r1 = _r1;
	*r2 = _r2;
}