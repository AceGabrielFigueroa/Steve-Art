#include "stdafx.h"
#include "EllipseBoard.h"

/*
Ellipse equations

Y axis is pos in downward direction
Rectangle W x H - upper left corner at(x, y)

X - Semi - axis: a = W / 2
Y - Semi - axis : b = H / 2
c = a / b


Center at(x + a, y + b)

(x - a)^2 / a^2 + (y - b)^2 / b^2 = 1

b^2 * (x - a)^2 + a^2 * (y - b)^2 = a^2 * b^2

Solve for x given y

b^2 * (x - a)^2 = a^2 * b^2 - a^2 * (y - b)^2

(x - a)^2 = a^2 - (a^2 * (y - b)^2) / b^2

(x - a)^2 = a^2 - c^2 * (y - b)^2

(x - a) = +- sqrt(a^2 - c^2(y - b)^2)

d = sqrt(a^2 - c^2(y - b)^2)

x = a +- d

For(yy = 0; yy < H; yy++)
{
	d = floor(sqrt(a^2 - c^2((y + 0.5) - b)^2))

		For(xx = 0; xx < a - d; xx++)		//	Outside circle
		For(xx = a - d; xx < a + d; xx++)	//	Inside circle
		For(xx = a + d; xx < W; xx++)		//	Outside circle
}
*/

EllipseBoard::EllipseBoard()
{
}

EllipseBoard::EllipseBoard(int width, int height, int columns, int rows, boolean randomSquare)
{
	Construct(width, height, columns, rows);
	//m_columns = columns;
	//m_rows = rows;
	m_random = randomSquare;
	Draw();
}

EllipseBoard::~EllipseBoard()
{
}

void EllipseBoard::Draw()
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

void EllipseBoard::Fill(int row, int col, int wid, int hgt)
{
	int r = row * hgt;
	int c = col * wid;
	int w = wid;
	int h = hgt;

	switch (irand(9))
	{
	case 0: UL_Q(r, c, w, h); break;
	case 1: UR_Q(r, c, w, h); break;
	case 2: LL_Q(r, c, w, h); break;
	case 3: LR_Q(r, c, w, h); break;
	case 4: TopEdgeHalfEllipse(r, c, w, h); break;
	case 5: BtmEdgeHalfEllipse(r, c, w, h); break;
	case 6: LeftEdgeHalfEllipse(r, c, w, h); break;
	case 7: RightEdgeHalfEllipse(r, c, w, h); break;
	case 8: FillFullEllipse(r, c, w, h); break;
	}
}

void EllipseBoard::FillFullEllipse(int row, int col, int wid, int hgt)
{
	double a = wid / 2.0;
	double b = hgt / 2.0;
	double c = a / b;
	//double aa = a * a;
	//double c2 = c * c;

	boolean outer = m_random ? ((rand() % 2) == 0 ? true : false) : false;
	boolean inner = !outer;

	for (auto y = 0; y < hgt; y++)
	{
		int thisrow = row + y;

		double k = (double)(y - b + 0.5) / (double)b;
		double m =  a * sqrt(1.0 - pow(k, 2.0));
		double from = floor(a - m);
		double to = ceil(a + m);

		// Before Ellipse
		for ( auto x = 0; x<from; x++)			SetAt(thisrow, col + x, outer);
		// In Ellipse
		for (auto x = from; x < to; x++)		SetAt(thisrow, col + x, inner);
		// After Ellipse
		for (auto x = to; x < wid; x++)			SetAt(thisrow, col + x, outer);
	}
}

// center of quarter ellipse is upper left corner
// (positive y is down)
void EllipseBoard::FillQuarterEllipse(int row, int col, int wid, int hgt)
{
	boolean outer = m_random ? ((rand() % 2) == 0 ? true : false) : false;
	boolean inner = !outer;

	for (auto y = 0; y < hgt; y++)
	{
		int thisrow = row + y;

		double w = (double)wid;
		double h = (double)hgt;
		int to = (int) ((w / h)*sqrt(h*h - y*y));

		// In Ellipse
		for (auto x = 0; x < to; x++)		SetAt(thisrow, col + x, inner);
		// After Ellipse
		for (auto x = to; x < wid; x++)		SetAt(thisrow, col + x, outer);
	}
}

void EllipseBoard::UL_Q(int row, int col, int wid, int hgt)
{
	FillQuarterEllipse(row, col, wid, hgt);
}
void EllipseBoard::UR_Q(int row, int col, int wid, int hgt)
{
	EllipseBoard b;
	b.Construct(wid, hgt, 1, 1);
	//b.Construct(wid, hgt);
	//b.m_columns = 1;
	//b.m_rows = 1;
	b.UL_Q(0, 0, wid, hgt);
	FlipHoriz(&b, 0, 0, col, row, wid, hgt);
}

void EllipseBoard::LL_Q(int row, int col, int wid, int hgt)
{
	EllipseBoard b;
	b.Construct(wid, hgt, 1, 1);
	//b.Construct(wid, hgt);
	//b.m_columns = 1;
	//b.m_rows = 1;
	b.UL_Q(0, 0, wid, hgt);
	FlipVert(&b, 0, 0, col, row, wid, hgt);
}

void EllipseBoard::LR_Q(int row, int col, int wid, int hgt)
{
	EllipseBoard b;
	b.Construct(wid, hgt, 1, 1);
	//b.Construct(wid, hgt);
	//b.m_columns = 1;
	//b.m_rows = 1;
	b.UR_Q(0, 0, wid, hgt);

	//EllipseBoard b1;
	//b1.Construct(wid, hgt);
	//b1.m_columns = 1;
	//b1.m_rows = 1;

	//b1.FlipVert(&b, 0, 0, row, col, wid, hgt);

	FlipVert(&b, 0, 0, col, row, wid, hgt);

}

// center of half ellipse is middle of top edge
// (positive y is down)
void EllipseBoard::TopEdgeHalfEllipse(int row, int col, int wid, int hgt)
{
	boolean outer = m_random ? ((rand() % 2) == 0 ? true : false) : false;
	boolean inner = !outer;

	for (auto y = 0; y < hgt; y++)
	{
		int thisrow = row + y;

		double w = (double)wid;
		double h = (double)hgt;
		int edge= (int)((w /(2*h))*sqrt(h*h - y * y));

		// Before Ellipse
		for (auto x = 0; x < edge; x++)			SetAt(thisrow, col + x, outer);
		// In Ellipse
		for (auto x = edge; x < wid-edge; x++)	SetAt(thisrow, col + x, inner);
		// After Ellipse
		for (auto x = wid-edge ; x < wid; x++)	SetAt(thisrow, col + x, outer);


		//// Before Ellipse
		//for (auto x = 0; x < from; x++)			SetAt(thisrow, col + x, outer);
		//// In Ellipse
		//for (auto x = from; x < to; x++)		SetAt(thisrow, col + x, inner);
		//// After Ellipse
		//for (auto x = to; x < wid; x++)			SetAt(thisrow, col + x, outer);
	}
}

void EllipseBoard::BtmEdgeHalfEllipse(int row, int col, int wid, int hgt)
{
	EllipseBoard b;
	b.Construct(wid, hgt,1,1);
	//b.Construct(wid, hgt);
	//b.m_columns = 1;
	//b.m_rows = 1;
	b.TopEdgeHalfEllipse(0, 0, wid, hgt);
	FlipVert(&b, 0, 0, col, row, wid, hgt);
}

void EllipseBoard::LeftEdgeHalfEllipse(int row, int col, int wid, int hgt)
{
	boolean outer = m_random ? ((rand() % 2) == 0 ? true : false) : false;
	boolean inner = !outer;
	double w = (double)wid;
	double h = (double)hgt;

	for (auto y = 0; y < hgt; y++)
	{
		int thisrow = row + y;

		int to = (int)((w / h)*sqrt(h*h - y*y/4));

		// In Ellipse
		for (auto x = 0; x < to; x++)		SetAt(thisrow, col + x, inner);
		// After Ellipse
		for (auto x = to; x < wid; x++)		SetAt(thisrow, col + x, outer);
	}
}

void EllipseBoard::RightEdgeHalfEllipse(int row, int col, int wid, int hgt)
{
	EllipseBoard b;
	b.Construct(wid, hgt, 1, 1);
	//b.Construct(wid, hgt);
	//b.m_columns = 1;
	//b.m_rows = 1;
	b.LeftEdgeHalfEllipse(0, 0, wid, hgt);
	FlipHoriz(&b, 0, 0, col, row, wid, hgt);
}


