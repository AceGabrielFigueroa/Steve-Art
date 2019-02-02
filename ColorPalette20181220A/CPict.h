#pragma once
#include <afxwin.h>
#include <vector>

class CPict : public CStatic
{
public:
	void ExOnPaint();
	void CPict::Clear();
	void AddCellRect(int x, int y, int wid, int hgt, COLORREF clr);
	void ClearCellRects();
	void Paint(CPaintDC dc);
	int SaveDCToBitmap(const char *pszFile);

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

	class CellRect
	{
	public:
		//CellRect(int x, int y, int wid, int hgt, COLORREF clr);
		//CellRect();
		//~CellRect();

		CellRect()
		{
			CellRect(0, 0, 0, 0, 0);
		}

		CellRect(int x, int y, int wid, int hgt, COLORREF clr)
		{
			m_x = x;
			m_y = y;
			m_wid = wid;
			m_hgt = hgt;
			m_clr = clr;
		}


		~CellRect()
		{
		}


		int m_x;
		int m_y;
		int m_wid;
		int m_hgt;
		COLORREF m_clr;
	};


	std::vector<CellRect> cellRects;
};

