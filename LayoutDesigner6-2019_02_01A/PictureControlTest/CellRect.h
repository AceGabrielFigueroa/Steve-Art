#pragma once
class CellRect
{
public:
	CellRect(int x, int y, int wid, int hgt, COLORREF clr);
	CellRect();
	~CellRect();

	int m_x;
	int m_y;
	int m_wid;
	int m_hgt;
	COLORREF m_clr;
};

