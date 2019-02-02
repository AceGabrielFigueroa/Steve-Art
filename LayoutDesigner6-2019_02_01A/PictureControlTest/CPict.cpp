#include "stdafx.h"
#include "CPict.h"
#include "CellRect.h"

#include <iostream>
using namespace std;


CPict::CPict()
{

	pParentDialog = NULL;
}

CPict::~CPict()
{
}

BEGIN_MESSAGE_MAP(CPict, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// Let Dialog handle this message
void CPict::OnLButtonDown(UINT nFlags, CPoint point)
{
	pParentDialog->SendMessage(WM_LBUTTONDOWN, 0, (LPARAM)this);
	CStatic::OnLButtonDown(nFlags, point);
}

// Let Dialog handle this message
void CPict::OnLButtonUp(UINT nFlags, CPoint point)
{
	pParentDialog->SendMessage(WM_LBUTTONUP, 0, (LPARAM)this);
	CStatic::OnLButtonUp(nFlags, point);
}

void CPict::ExOnPaint()
{
	OnPaint();
}

void CPict::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here

	for (int i = 0; i < (int) cellRects.size(); i++)
	{
		CellRect cr = cellRects[i];
		dc.FillSolidRect(cr.m_x, cr.m_y, cr.m_wid, cr.m_hgt, cr.m_clr);
		//char buffer[100];
		//sprintf_s(buffer, "OnPaint XY( %3d, %3d)  WH( %3d, %3d )    %9x\n", cr.m_x, cr.m_y, cr.m_wid, cr.m_hgt, cr.m_clr);
		//OutputDebugStringA(buffer);
	}

	CStatic::OnPaint();
}

void CPict::Paint(CPaintDC dc)
{
	for (int i = 0; i < (int) cellRects.size(); i++)
	{
		CellRect cr = cellRects[i];
		dc.FillSolidRect(cr.m_x, cr.m_y, cr.m_wid, cr.m_hgt, cr.m_clr);

		//char buffer[100];
		//sprintf_s(buffer, "OnPaint XY( %3d, %3d)  WH( %3d, %3d )    %9x\n", cr.m_x, cr.m_y, cr.m_wid, cr.m_hgt, cr.m_clr);
		//OutputDebugStringA(buffer);
	}

	CStatic::OnPaint();
}

void CPict::AddCellRect(int x, int y, int wid, int hgt, COLORREF clr)
{
	cellRects.emplace_back(x, y, wid, hgt, clr);
}

void CPict::ClearCellRects()
{
	cellRects.clear();
	Clear();
}

void CPict::Clear()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetWindowRect(rect);
	int wid = rect.Width();
	int hgt = rect.Height();

	dc.FillSolidRect(0, 0, wid, hgt, RGB(255, 255, 255));

	char buffer[100];
	sprintf_s(buffer, "CPict::Clear  WH( %3d, %3d )\n", wid, hgt );
	OutputDebugStringA(buffer);

	CStatic::OnPaint();
}