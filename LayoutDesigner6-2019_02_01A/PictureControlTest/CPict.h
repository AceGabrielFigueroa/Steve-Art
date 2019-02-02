#pragma once
#include <afxwin.h>
#include <vector>
#include "CellRect.h"

class CPict : public CStatic
{
public:
	void ExOnPaint();
	void CPict::Clear();

	std::vector<CellRect> cellRects;

	void AddCellRect(int x, int y, int wid, int hgt, COLORREF clr);
	void ClearCellRects();
	void Paint(CPaintDC dc);
	CWnd *pParentDialog;


public:
	CPict();
	~CPict();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

