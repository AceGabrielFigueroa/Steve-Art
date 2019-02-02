#include "stdafx.h"
#include "CellRect.h"


CellRect::CellRect()
{
	CellRect(0, 0, 0, 0, 0);
}

CellRect::CellRect(int x, int y, int wid, int hgt, COLORREF clr)
{
	m_x = x;
	m_y = y;
	m_wid = wid;
	m_hgt = hgt;
	m_clr = clr;
}


CellRect::~CellRect()
{
}
