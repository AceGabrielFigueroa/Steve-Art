#include "stdafx.h"
#include "CPict.h"

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
	if (pParentDialog != NULL)
		pParentDialog->SendMessage(WM_LBUTTONDOWN, 0, (LPARAM)this);
	CStatic::OnLButtonDown(nFlags, point);
}

// Let Dialog handle this message
void CPict::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (pParentDialog != NULL)
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

	try
	{
		CStatic::OnPaint();
		//Invalidate();
	}
	catch (...)
	{
	}
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

	//char buffer[100];
	//sprintf_s(buffer, "CPict::Clear  WH( %3d, %3d )\n", wid, hgt );
	//OutputDebugStringA(buffer);

	CStatic::OnPaint();
}

int CPict::SaveDCToBitmap(const char *pszFile)
{

	CRect rect;
	GetWindowRect(rect);

	CDC *dc = GetDC();

	BITMAPFILEHEADER bmfh = { 0 };
	BITMAPINFOHEADER bmih = { 0 };
	BITMAPINFO bi;

	HDC hdc1 = *dc;
	HDC hdc2 = CreateCompatibleDC(hdc1);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = w;
	bmih.biHeight = h;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = ((((bmih.biWidth * bmih.biBitCount) + 31) & ~31) >> 3) * bmih.biHeight;
	bi.bmiHeader = bmih;

	void *dibvalues;
	HBITMAP aBmp = CreateDIBSection(hdc1, &bi, DIB_RGB_COLORS, (void**)&dibvalues, NULL, NULL);
	if (aBmp == 0)
	{
		AfxMessageBox(_T("Failure to create Bitmap\n"));
		return -1;
	}

	HGDIOBJ OldObj = SelectObject(hdc2, aBmp);
	BitBlt(hdc2, 0, 0, w, h, hdc1, 0, 0, SRCCOPY);
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfh.bfSize = (3 * bmih.biHeight*bmih.biWidth) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfh.bfType = 0x4d42;

	HANDLE fileHandle = CreateFileA(pszFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD bytes_written;
	DWORD bytes_write = sizeof(BITMAPFILEHEADER);
	WriteFile(fileHandle, &bmfh, bytes_write, &bytes_written, NULL);

	bytes_write = sizeof(BITMAPINFOHEADER);
	WriteFile(fileHandle, &bmih, bytes_write, &bytes_written, NULL);

	bytes_write = bmih.biSizeImage;
	WriteFile(fileHandle, (void*)dibvalues, bytes_write, &bytes_written, NULL);

	CloseHandle(fileHandle);
	DeleteObject(SelectObject(hdc2, OldObj));
	DeleteDC(hdc2);
	ReleaseDC(dc);
	return 0;
}