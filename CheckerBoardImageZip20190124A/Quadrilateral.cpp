#include "stdafx.h"
#include "Quadrilateral.h"


Quadrilateral::Quadrilateral()
{
}


Quadrilateral::~Quadrilateral()
{
}


Quadrilateral::Quadrilateral(int width, int height, int columns, int rows, boolean randomSquare)
{
	Construct(width, height, columns, rows);
	m_random = randomSquare;
	Draw();
}

// A utility function to calculate area of quadrangle inscribed in a rectangle
// Take area of rectangle and then subtract the corner triangles, 
// leaving the area of the quadrilateral
float quadArea(int xa, int ya, int xb, int yb, int xc, int yc, int xd, int yd, int wid, int hgt)
{
	int A = wid * hgt;
	return A - 0.5 * (xa * yb + (wid - xa)*yd + (hgt - yb)*xc + (hgt - yd)*(wid - xc));
}

void Quadrilateral::Draw()
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

// From standard equation of a line:
// Given y - Find X
//
// The algorithm goes row by row
// In each row, write a bool value starting at the left edge
// up to the line.  Then write the opposite value
int Quadrilateral::xValue(int x1, int y1, int x2, int y2, int y)
{
	double slope = (y2 - y1) / (double)(x2 - x1);
	return abs((int)(x1 + (y - y1) / slope));
}

// ...................A....................
// .................******.................
// ................***********.............
// ...............****************.........
// ..............*********************.....
// .............**************************D
// ............**************************..
// ...........***************************..
// ..........***************************...
// ........*****************************...
// .......*****************************....
// ......******************************....
// .....*******************************....
// ....*******************************.....
// ...********************************.....
// ..********************************......
// .*********************************......
// B********************************.......
// ................*****************.......
// .................................C......

void Quadrilateral::Fill(int row, int col, int wid, int hgt)
{
	int r = row * hgt;
	int c = col * wid;

	const double minQuadArea = wid * hgt / 4.0;
	boolean inner = rand() % 2 == 0;
	boolean outer = !inner;
	float fillArea = wid * hgt;

	// Check for minimum size 
	// -- if less, than fill whole block with the same bool value
	if (wid < 5 || hgt < 5)
	{
		boolean value = rand() % 2 == 0;
		for (auto y = 0; y < hgt; y++)
		{
			int thisrow = r + y;
			for (auto x = 0; x < wid; x++)
				SetAt(thisrow, c + x,	value);

		}
	} // End small quads
	else
	{
		//////////////////////////////////////////////////
		//
		// Randomly find points on edges
		//
		// Quad vertices for points A, B, C D.
		// A is at top, B on left edge, C on bottom, D on right edge
		int xa, ya, xb, yb, xc, yc, xd, yd;

		// Want Quad to be at least the value min Quad Area, 
		// to avoid very skinny ones.
		// For small containing rectangles, it may be inmpractical to
		// get a quad of minimum size. So, put a limit
		// so limit number of tries 
		// (Redundant with above, but just  protection for crazy case with infinit loop
		int limit = 20;
		int tries = 0;
		float A;

		ya = 0;			// (xa, ya) - Top Edge
		xb = 0;			// (xb, yb) - Left Edge
		yc = hgt - 1;	// (xc, yc) - Bottom Edge
		xd = wid - 1;	// (xd, yd) - Right Edge

		do {
			xa = irand( 1, wid - 2 );	// (xa, ya) - Top Edge
			yb = irand( 1, hgt - 2 );	// (xb, yb) - Left Edge
			xc = irand( 1, wid - 2 );	// (xc, yc) - Bottom Edge
			yd = irand( 1, hgt - 2 );	// (xd, yd) - Right Edge

			tries++;
			A = quadArea(xa, ya, xb, yb, xc, yc, xd, yd, wid, hgt);
		} while ((A < minQuadArea || xa == xc || yb == yd) 
			&& tries < limit);
		//  End Find Quadrilateral coordinates
		////////////////////////////////////////////////////////
		//
		// Fill Quadrilateral

		int xab, xad, xdc, xbc;
		// Two possible orientations
		//		(Note selection algorithm guarantees that 
		//		A & C have diff x and B & D have diff y)
		if (yb < yd)
		{
			//---------------
			//     A
			//  +--*-----+
			//  |        |
			//  |        |
			//B *        |
			//  |        |
			//  |        * D
			//  |        |
			//  |        |
			//  |        |
			//  +---*----+
			//     C
			//-----------------

			// Before B
			for (auto y = 0; y < yb; y++)
			{
				xab = xValue(xa, ya, xb, yb, y);
				xad = xValue(xa, ya, xd, yd, y);
				FillLine(xab, xad, wid, r + y, c, outer);
			}

			// Between B and D
			for (auto y = yb; y < yd; y++)
			{
				xbc = xValue(xb, yb, xc, yc, y);
				xad = xValue(xa, ya, xd, yd, y);
				FillLine(xbc, xad, wid, r + y, c, outer);
			}

			// After D
			for (auto y = yd; y < hgt; y++)
			{
				xbc = xValue(xb, yb, xc, yc, y);
				xdc = xValue(xd, yd, xc, yc, y);
				FillLine(xbc, xdc, wid, r + y, c, outer);
			}
		}
		else
		{
			//---------------
			//     A
			//  +--*-----+
			//  |        |
			//  |        * D
			//  |        |
			//  |        |
			//  |        |
			//B *        |
			//  |        |
			//  |        |
			//  +---*----+
			//     C
			//-----------------

			// Before D
			
			for (auto y = 0; y < yd; y++)
			{
				xab = xValue(xa, ya, xb, yb, y);
				xad = xValue(xa, ya, xd, yd, y);
				FillLine(xab, xad, wid, r + y, c, outer);
			}
			// Between D and B
			for (auto y = yd; y < yb; y++)
			{
				xab = xValue(xa, ya, xb, yb, y);
				xdc = xValue(xd, yd, xc, yc, y);
				FillLine(xab, xdc, wid, r + y, c, outer);
			}
			// After B			
			for (auto y = yb; y < hgt; y++)
			{
				xbc = xValue(xc, yc, xb, yb, y);
				xdc = xValue(xd, yd, xc, yc, y);
				FillLine(xbc, xdc, wid, r + y, c, outer);
			}
		}
	} // End of quads that are above minimum size
}

void Quadrilateral::FillLine(int xleft, int xright, int wid, int row, int col, boolean outer)
{
	if (xleft == xright)
	{
		for (auto x = 0; x < wid; x++)		
			SetAt(row, col + x, outer);
		SetAt(row, xleft, !outer);
	}
	else
	{
		for (auto x = 0; x < xleft; x++)		SetAt(row, col + x, outer);
		for (auto x = xleft; x < xright; x++)	SetAt(row, col + x, !outer);
		for (auto x = xright; x < wid; x++)		SetAt(row, col + x, outer);
	}
}
//char buffer[200];
//sprintf_s(buffer, "Quad: HW (%3d, %3d) A(%3d,%3d) B(%3d,%3d) C(%3d,%3d) D(%3d,%3d) QuadArea %6.0f\n",
//	hgt, wid, xa, ya, xb, yb, xc, yc, xd, yd, A );
//OutputDebugStringA(buffer);

