// PictureControlTestDlg.h : header file
//
#pragma once
#include "CPict.h"
#include "LayoutRect.h"
#include <vector>

// CPictureControlTestDlg dialog
class CPictureControlTestDlg : public CDialogEx
{
// Construction
public:
	CPictureControlTestDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICTURECONTROLTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CPict m_picture;
	afx_msg void OnBnClickedAddRectangle();
	void AddCellRect(int x, int y, int wid, int hgt, COLORREF clr);
	void AddRectangle(int fromx, int fromy, int tox, int toy);

private:

	const int SAFETY_VALUE = 13572468;
	COLORREF INTERIOR_FREE = RGB(255, 255, 255);
	COLORREF INTERIOR_USED = RGB( 220, 255, 220 );
	COLORREF INTERIOR_EDGE = RGB(230, 210, 210);
	COLORREF CELL_EDGE = RGB( 200, 128, 128 );
	COLORREF RECTANGLE_EDGE = RGB(0, 0, 0);;
	int LINE_WIDTH = 3;

	int max_cells_vert = 9;		// broke > 9  was 16;
	int max_cells_horiz = 9;		// broke > 9  was 16;
	bool cellsFree[16][16];
	char buffer[200];

	int picture_width;
	int picture_height;
	int cells_horiz;
	int cells_vert;
	int cell_width;
	int cell_height;

	int mouseDownX = 0;
	int mouseDownY = 0;
	std::vector<LayoutRect> layoutRects;

	CString readLine;
	CString fileName;

	CEdit m_from_x;
	CEdit m_from_y;
	CEdit m_to_x;
	CEdit m_to_y;
	CEdit m_cells_horiz;
	CEdit m_cells_vert;


	void SetUpLayout();
	void PaintCellLeftEdge(int cellx, int celly, COLORREF clr);
	void PaintCellRightEdge(int cellx, int celly, COLORREF clr);
	void PaintCellTopEdge(int cellx, int celly, COLORREF clr);
	void PaintCellBottomEdge(int cellx, int celly, COLORREF clr);
	void PaintCellInterior(int cellx, int celly, COLORREF clr);
	void PaintRectangle(int fromx, int fromy, int tox, int toy);

	boolean FileNameIsAvailable(CString filename);

	bool DisplayRectangleList();
	bool IsLayoutComplete();
	void ResetEditControls();
	void EnableEditCtrls(boolean fromx, boolean fromy, boolean tox, boolean toy);
	boolean CEditIsEmpty(CEdit *ctrl);
	void SetEdit(CEdit *edit, int value);
	
	int CheckParamFromEdit(CEdit *edit, CString toOrFrom, CString xOrY, int max);
	void Reset();

	boolean SaveRectanglesToFile();
	boolean ReadRectsFromFile();
	void GetNextLine(CStdioFile *file );
	void ConvertCString(CString s, char chrs[]);
	void ConvertCString(CString s, TCHAR chrs[]);
	void SetCellsFree(boolean trueOrFalse);
	void SaveLayoutImage(CString fname);
	void SaveLayoutImage();
	int SaveDCToBitmap(const char *pszFile, RECT rc, CDC *pDC);
	void SetNewGridLayout();
	void SetNewGridLayout(std::vector<LayoutRect> list);
	void SaveReset(boolean reset);
	void VerifyLayoutList(std::vector<LayoutRect> &list);
	void PrintLayoutList(std::vector<LayoutRect> list);
	void PaintRects();
	void PaintRects(std::vector<LayoutRect> list);
	boolean IsMouseInGrid();
	CPoint GetCellXY();

	boolean SaveRectanglesToFile(std::vector<LayoutRect>, CString filenm);
	boolean EqualLayouts(std::vector<LayoutRect> list1, std::vector<LayoutRect> list2);
	void SaveSymmetricalLayout(std::vector<LayoutRect>& list, CString filename);
	void SortLayoutList(std::vector<LayoutRect> &list);
	std::vector<LayoutRect> ReflectHoriz(std::vector<LayoutRect> list);
	std::vector<LayoutRect> ReflectVert(std::vector<LayoutRect> list);
	std::vector<LayoutRect> RotateLeft(std::vector<LayoutRect> list);

public:
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonCreategrid();
	afx_msg void OnStnClickedStaticPicture();
	afx_msg void OnStnDblclickStaticPicture();
	afx_msg void OnBnClickedButtonRotateLeft();
	afx_msg void OnBnClickedButtonReflectVert();
	afx_msg void OnBnClickedButtonReflectHoriz();
	afx_msg void OnBnClickedButtonDeleteLast();
	afx_msg void OnBnClickedButtonSaveReset();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonSymmetries();
};
