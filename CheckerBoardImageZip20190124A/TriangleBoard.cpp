#include "stdafx.h"
#include "TriangleBoard.h"


TriangleBoard::TriangleBoard()
{
}

TriangleBoard::TriangleBoard(int width, int height, int columns, int rows, boolean randomSquare)
{
	Construct(width, height, columns, rows);
	m_random = randomSquare;
	Draw();
}


TriangleBoard::~TriangleBoard()
{
}


/* A utility function to calculate area of triangle formed by (x1, y1),
   (x2, y2) and (x3, y3) */
float area(int x1, int y1, int x2, int y2, int x3, int y3)
{
	return abs((x1*(y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

/* A function to check whether point P(x, y) lies inside the triangle formed
   by A(x1, y1), B(x2, y2) and C(x3, y3) */
bool isInside(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
{
	/* Calculate area of triangle ABC */
	float A = area(x1, y1, x2, y2, x3, y3);

	/* Calculate area of triangle PBC */
	float A1 = area(x, y, x2, y2, x3, y3);

	/* Calculate area of triangle PAC */
	float A2 = area(x1, y1, x, y, x3, y3);

	/* Calculate area of triangle PAB */
	float A3 = area(x1, y1, x2, y2, x, y);

	/* Check if sum of A1, A2 and A3 is same as A */
	return (A == A1 + A2 + A3);
}

bool isInside(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y, float A)
{
	/* Calculate area of triangle ABC */
	//float A = area(x1, y1, x2, y2, x3, y3);

	/* Calculate area of triangle PBC */
	float A1 = area(x, y, x2, y2, x3, y3);

	/* Calculate area of triangle PAC */
	float A2 = area(x1, y1, x, y, x3, y3);

	/* Calculate area of triangle PAB */
	float A3 = area(x1, y1, x2, y2, x, y);

	/* Check if sum of A1, A2 and A3 is same as A */
	return (A == A1 + A2 + A3);
}


void TriangleBoard::Draw()
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

void TriangleBoard::Fill(int row, int col, int wid, int hgt)
{
	enum {	UL, LL, UR, LR, 
			TOP_ISO, BTM_ISO, RGT_ISO, LFT_ISO, 
			RAND_INT, 
			TOP_RAND, BTM_RAND, RGT_RAND, LFT_RAND,
			N};

	int r = row * hgt;
	int c = col * wid;
	int w = wid;
	int h = hgt;

	//if (m_random)
	//{
		switch (rand() % N)
		{
		case UL:
			ULTriangle(r, c, w, h);
			break;
		case UR:
			URTriangle(r, c, w, h);
			break;
		case LL:
			LLTriangle(r, c, w, h);
			break;
		case LR:
			LRTriangle(r, c, w, h);
			break;
	
		case TOP_ISO:
			TopEdgeIsoceles(r, c, w, h);
			break;
		case BTM_ISO:
			BtmEdgesoceles(r, c, w, h);
			break;
		case LFT_ISO:
			LftEdgeIsoceles(r, c, w, h);
			break;
		case RGT_ISO:
			RgtEdgeIsoceles(r, c, w, h);
			break;

		case RAND_INT:
			RandomTriangle(r, c, w, h);
			break;

		case TOP_RAND:
			TopEdgeRandom(r, c, w, h);
			break;
		case BTM_RAND:
			BtmEdgeRandom(r, c, w, h);
			break; 
		case RGT_RAND:
			RgtEdgeRandom(r, c, w, h);
			break;		
		case LFT_RAND:
			LftEdgeRandom(r, c, w, h);
			break;
		default:
			ASSERT(false);
		}
//	}
//	else
		//RandomTriangle(r, c, w, h);
		//TopEdgeRandom(r, c, w, h);
}

// Four possible orientations
//
//      UL      UR      LL      LR
//
//     *****   *****   *....   ....*
//     ****.   .****   **...   ...**
//     ***..   ..***   ***..   ..***
//     **...   ...**   ****.   .****
//     *....   ....*   *****   *****

void TriangleBoard::ULTriangle(int row, int col, int wid, int hgt)
{
	double slope = ((double)wid) / hgt;
	// 
	// Fill in first r
	for (auto c = 0; c < wid; c++)
		SetAt(row, col + c, true);

	// For rows up till last use formula
	for (auto r = 1; r < hgt - 1; r++)
	{
		int n = wid - (int) ceil(slope * r);
		int thisrow = row + r;
		for ( auto c=0; c<n; c++)
			SetAt(thisrow, col+c, true);
		for (auto c = n; c < wid; c++)
			SetAt(thisrow, col + c, false);
	}

	// Last r -- only set first one
	int lastrow = row + hgt - 1;
	SetAt(lastrow, col, true);
	for (auto c = 1; c < wid; c++)
		SetAt(lastrow, col + c, false);
}

void TriangleBoard::LRTriangle(int row, int col, int wid, int hgt)
{
	double slope = ((double)wid) / hgt;
	// 
	// First r -- only set last one
	SetAt(row, col+wid-1, true);
	for (auto c = 0; c < wid-1; c++)
		SetAt(row, col + c, false);

	// For rows up till last use formula
	for (auto r = 1; r < hgt - 1; r++)
	{
		int n = wid - (int)ceil(slope * r);
		int thisrow = row + r;
		for (auto c = 0; c < n; c++)
			SetAt(thisrow, col + c, false);
		for (auto c = n; c < wid; c++)
			SetAt(thisrow, col + c, true);
	}

	// Last r -- set all
	int lastrow = row + hgt - 1;
	for (auto c = 0; c < wid; c++)
		SetAt(lastrow, col + c, true);
}

void TriangleBoard::URTriangle(int row, int col, int wid, int hgt)
{
	double slope = ((double)wid) / hgt;
	// 
	// Fill in first r
	for (auto c = 0; c < wid; c++)
		SetAt(row, col + c, true);

	// For rows up till last use formula
	for (auto r = 1; r < hgt - 1; r++)
	{
		int n = (int)ceil(slope * r);
		int thisrow = row + r;
		for (auto c = 0; c < n; c++)
			SetAt(thisrow, col + c, false);
		for (auto c = n; c < wid; c++)
			SetAt(thisrow, col + c, true);
	}

	// Last r -- only set last one
	int lastrow = row + hgt - 1;
	for (auto c = 0; c < wid-1; c++)
		SetAt(lastrow, col + c, false);
	SetAt(lastrow, col+wid-1, true);
}

void TriangleBoard::LLTriangle(int row, int col, int wid, int hgt)
{
	double slope = ((double)wid) / hgt;
	// 
	// First r  - only set first one
	SetAt(row, col, true);
	for (auto c = 1; c < wid; c++)
		SetAt(row, col + c, false);

	// For rows up till last use formula
	for (auto r = 1; r < hgt - 1; r++)
	{
		int n = (int)floor(slope * r);
		int thisrow = row + r;
		for (auto c = 0; c < n; c++)
			SetAt(thisrow, col + c, true);
		for (auto c = n; c < wid; c++)
			SetAt(thisrow, col + c, false);
	}

	// Last r -- set all
	int lastrow = row + hgt - 1;
	for (auto c = 0; c < wid; c++)
		SetAt(lastrow, col + c, true);
}

void TriangleBoard::TopEdgeIsoceles(int row, int col, int wid, int hgt)
{
	boolean outer = irand(2) == 0;
	boolean inner = !outer;

	double slope = ((double)wid) / (2.0*hgt);
	// 
	// Fill in first r
	for (auto c = 0; c < wid; c++)
		SetAt(row, col + c, true);

	// For rows up till last use formula
	for (auto r = 1; r < hgt; r++)
	{
		int n = wid/2 - (int)ceil(slope * r);
		if (n < 0) n = 0;
		int thisrow = row + r;

		// Before Triangle
		for (auto c = 0; c < n; c++)			SetAt(thisrow, col + c, outer);
		// In Triangle
		for (auto c = n; c < wid-n; c++)		SetAt(thisrow, col + c, inner);
		// After Triangle
		for (auto c = wid-n; c < wid; c++)		SetAt(thisrow, col + c, outer);
	}

	//// Last row -- only set first one
	//int lastrow = r + h - 1;
	//SetAt(lastrow, c, true);
	//for (auto c = 1; c < wid; c++)
	//	SetAt(lastrow, col + c, false);
}

void TriangleBoard::BtmEdgesoceles(int row, int col, int wid, int hgt)
{
	TriangleBoard b;
	b.Construct(wid, hgt, 1, 1);
	//b.Construct(wid, hgt);
	//b.m_columns = 1;
	//b.m_rows = 1;
	b.TopEdgeIsoceles(0, 0, wid, hgt);
	FlipVert(&b, 0, 0, col, row, wid, hgt);
}

void TriangleBoard::LftEdgeIsoceles(int row, int col, int wid, int hgt)
{
	boolean outer = m_random ? ((rand() % 2) == 0 ? true : false) : false;
	boolean inner = !outer;
	double w = (double)wid;
	double h = (double)hgt;

	double slope = 2.0 * w / h;

	// Top Half
	for (auto r = 0; r < hgt/2; r++)
	{
		int thisrow = row + r;
		int to = wid - (int)(slope*r);

		// In Ellipse
		for (auto x = 0; x < to; x++)		SetAt(thisrow, col + x, inner);
		// After Ellipse
		for (auto x = to; x < wid; x++)		SetAt(thisrow, col + x, outer);
	}

	// If height is odd - need a r of all inner values
	int firstRowBtmHalf = 0;
	if (hgt % 2 == 1) 
	{
		int midrow = hgt / 2;
		for (auto c = 0; c < wid; c++)		SetAt(midrow, c, inner);
		firstRowBtmHalf = midrow + 1;
	}
	else
		firstRowBtmHalf = hgt / 2;

	// Bottom Half
	for (auto r = firstRowBtmHalf; r < hgt; r++)
	{
		int thisrow = row + r;
		int to = wid - (int)(slope*(hgt-r));

		// In Triangle
		for (auto x = 0; x < to; x++)		SetAt(thisrow, col + x, inner);
		// After Triangle
		for (auto x = to; x < wid; x++)		SetAt(thisrow, col + x, outer);
	}
}

void TriangleBoard::RgtEdgeIsoceles(int row, int col, int wid, int hgt)
{
	TriangleBoard b;
	b.Construct(wid, hgt, 1, 1);
	b.LftEdgeIsoceles(0, 0, wid, hgt);
	FlipHoriz(&b, 0, 0, col, row, wid, hgt);
}

void TriangleBoard::RandomTriangle(int row, int col, int wid, int hgt)
{
	const double minTriArea = wid * hgt / 4.0;
	boolean inner = rand() % 2 == 0;
	boolean outer = !inner;

	// Triangle vertices
	int x1, y1, x2, y2, x3, y3;
	// Want Triangle to be at least the value minTri Area, 
	// to avoid very skinny ones.
	// For very small containing rectangles, it may be impossible to
	// get a triangle of desired size.
	// so limit number of tries
	int limit = 20;
	int tries = 0;
	float A;
	do {
		x1 = rand() % wid;	// (x1, y1)
		y1 = rand() % hgt;
		x2 = rand() % wid;	// (x2, y2)
		y2 = rand() % hgt;
		x3 = rand() % wid;	// (x3, y3)
		y3 = rand() % hgt;
		tries++;
		A = area(x1, y1, x2, y2, x3, y3);
	} while ( A < minTriArea && tries < limit);

	for (auto y = 0; y < hgt ; y++)
	{
		int thisrow = row + y;
		for (auto x = 0; x < wid; x++)
			SetAt(thisrow, col + x, 
				isInside(x1, y1, x2, y2, x3, y3, x, y, A)? inner : outer);

	}
}

// This version uses one side as the bas and a random point on the opposite edge
// to define the triangle.
void TriangleBoard::TopEdgeRandom(int row, int col, int wid, int hgt)
{
	boolean inner = rand() % 2 == 0;
	boolean outer = !inner;

	int bottomX = irand(wid);

	double leftSlope = ((double)bottomX) / hgt; 
	double rightSlope = ((double)(wid-bottomX)) / hgt;
	
	// Fill in first row
	for (auto c = 0; c < wid; c++)
		SetAt(row, col + c, true);

	// For rows up till last use formula
	for (auto r = 1; r < hgt; r++)
	{
		int thisrow = row + r;
		int before = bottomX - (int)ceil(leftSlope * r);
		int after = rightSlope * (r - (hgt - 1)) + wid - 1;

		// Before Triangle
		for (auto c = 0; c < before; c++)		SetAt(thisrow, col + c, outer);
		// In Triangle
		for (auto c = before; c <after; c++)	SetAt(thisrow, col + c, inner);
		// After Triangle
		for (auto c = after; c < wid; c++)		SetAt(thisrow, col + c, outer);
	}
}


void TriangleBoard::BtmEdgeRandom(int row, int col, int wid, int hgt)
{
	TriangleBoard b;
	b.Construct(wid, hgt, 1, 1);
	b.TopEdgeRandom(0, 0, wid, hgt);
	FlipVert(&b, 0, 0, col, row, wid, hgt);
}

void TriangleBoard::LftEdgeRandom(int row, int col, int wid, int hgt)
{
	boolean outer = m_random ? ((rand() % 2) == 0 ? true : false) : false;
	boolean inner = !outer;
	double w = (double)wid;
	double h = (double)hgt;

	int rightY = irand(hgt);

	double topSlope = ((double)rightY) / hgt;
	double btmSlope = ((double)(hgt - rightY)) / hgt;

	// Top Half
	for (auto r = 0; r < rightY; r++)
	{
		int thisrow = row + r;
		int to = wid - (int)(topSlope*r);
		if (to < 0) to = 0;

		// In Triangle
		for (auto x = 0; x < to; x++)		SetAt(thisrow, col + x, inner);
		// After Triangle
		for (auto x = to; x < wid; x++)		SetAt(thisrow, col + x, outer);
	}

	// If height is odd - need a row of all inner values
	int firstRowBtmHalf = 0;
	if (hgt % 2 == 1)
	{
		int midrow = hgt / 2;
		for (auto c = 0; c < wid; c++)		SetAt(midrow, c, inner);
		firstRowBtmHalf = midrow + 1;
	}
	else
		firstRowBtmHalf = hgt / 2;

	// Bottom Half
	for (auto r = firstRowBtmHalf; r < hgt; r++)
	{
		int thisrow = row + r;
		int to = wid - (int)(btmSlope*(hgt - r));
		if (to < 0) to = 0;

		// In Triangle
		for (auto x = 0; x < to; x++)		SetAt(thisrow, col + x, inner);
		// After Triangle
		for (auto x = to; x < wid; x++)		SetAt(thisrow, col + x, outer);
	}
}

void TriangleBoard::RgtEdgeRandom(int row, int col, int wid, int hgt)
{
	TriangleBoard b;
	b.Construct(wid, hgt, 1, 1);
	b.LftEdgeRandom(0, 0, wid, hgt);
	FlipHoriz(&b, 0, 0, col, row, wid, hgt);
}