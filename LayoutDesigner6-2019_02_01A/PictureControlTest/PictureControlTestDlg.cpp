// PictureControlTestDlg.cpp : implementation file
//

//using namespace std;

#include "stdafx.h"
#include "PictureControlTest.h"
#include "PictureControlTestDlg.h"
#include "afxdialogex.h"
#include "CPict.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPictureControlTestDlg::CPictureControlTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PICTURECONTROLTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPictureControlTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_FROM_X, m_from_x);
	DDX_Control(pDX, IDC_FROM_Y, m_from_y);
	DDX_Control(pDX, IDC_TO_X, m_to_x);
	DDX_Control(pDX, IDC_TO_Y, m_to_y);
	DDX_Control(pDX, IDC_EDIT_CELLS_HORIZ, m_cells_horiz);
	DDX_Control(pDX, IDC_EDIT_CELLS_VERT, m_cells_vert);
}

BEGIN_MESSAGE_MAP(CPictureControlTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_ADD_RECTANGLE, &CPictureControlTestDlg::OnBnClickedAddRectangle)
	ON_BN_CLICKED(IDC_RESET, &CPictureControlTestDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDOK, &CPictureControlTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CPictureControlTestDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CPictureControlTestDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_CREATEGRID, &CPictureControlTestDlg::OnBnClickedButtonCreategrid)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_LEFT, &CPictureControlTestDlg::OnBnClickedButtonRotateLeft)
	ON_BN_CLICKED(IDC_BUTTON_REFLECT_VERT, &CPictureControlTestDlg::OnBnClickedButtonReflectVert)
	ON_BN_CLICKED(IDC_BUTTON_REFLECT_HORIZ, &CPictureControlTestDlg::OnBnClickedButtonReflectHoriz)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_LAST, &CPictureControlTestDlg::OnBnClickedButtonDeleteLast)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_RESET, &CPictureControlTestDlg::OnBnClickedButtonSaveReset)
	ON_BN_CLICKED(IDC_BUTTON_SYMMETRIES, &CPictureControlTestDlg::OnBnClickedButtonSymmetries)
END_MESSAGE_MAP()


// CPictureControlTestDlg message handlers

BOOL CPictureControlTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	cells_vert  = 3;
	cells_horiz = 3;
	SetUpLayout();
	m_picture.Invalidate();
	m_picture.pParentDialog = this;

	SetCellsFree(true);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPictureControlTestDlg::SetUpLayout()
{
	m_picture.Clear();

	CRect rect;
	m_picture.GetWindowRect(rect);
	picture_width = rect.Width();
	picture_height = rect.Height();
	//cells_horiz = 3;
	//cells_vert = 3;
	cell_width = picture_width / cells_horiz;
	cell_height = picture_height / cells_vert;

	// Outer Edges
	// Top Edge
	m_picture.AddCellRect(0, 0, picture_width, LINE_WIDTH, RECTANGLE_EDGE);
	// Bottom Edge
	m_picture.AddCellRect(0, picture_height-LINE_WIDTH, picture_width, LINE_WIDTH, RECTANGLE_EDGE);
	// Left Side
	m_picture.AddCellRect(0, 0, LINE_WIDTH, picture_height, RECTANGLE_EDGE);
	// RightSide
	m_picture.AddCellRect(picture_width-LINE_WIDTH, 0, LINE_WIDTH, picture_height,  RECTANGLE_EDGE);

	// Inner Cell Boundaries
	for (int i = 0; i < cells_horiz; i++)
	{
		for (int j = 0; j < cells_vert; j++)
		{
			// Top, Middle or Bottom
			if (j == 0)
			{
				// Top Row
				PaintCellTopEdge(i, j, RECTANGLE_EDGE);
				PaintCellBottomEdge(i, j, CELL_EDGE);
			}
			else if (j < cells_vert - 1) {
				// Middle Rows
				PaintCellTopEdge(i, j, CELL_EDGE);
				PaintCellBottomEdge(i, j, CELL_EDGE);
			}
			else {
				// Bottom Row
				PaintCellTopEdge(i, j, CELL_EDGE);
				PaintCellBottomEdge(i, j, RECTANGLE_EDGE);
			}

			// Left, Middle, or Right
			if (i == 0)
			{
				// Left Column
				PaintCellLeftEdge(i, j, RECTANGLE_EDGE);
				PaintCellRightEdge(i, j, CELL_EDGE);
			}
			else if (i < cells_horiz - 1) {
				// Middle Column
				PaintCellLeftEdge(i, j, CELL_EDGE);
				PaintCellRightEdge(i, j, CELL_EDGE);
			}
			else {
				// Right Column
				PaintCellLeftEdge(i, j, CELL_EDGE);
				PaintCellRightEdge(i, j, RECTANGLE_EDGE);
			}

			PaintCellInterior(i, j, INTERIOR_FREE);
		}
	}
}

// Does not do the actual painting.
// Adds info into an array in the Grid layout picture control of areas 
// to be painted in the layout bitmap
void CPictureControlTestDlg::PaintRectangle(int fromx, int fromy, int tox, int toy)
{
	////////////////////////////////////////////////////
	// Cell Edges in Rectangle
	for (int x = fromx; x <= tox; x++)
	{
		for (int y = fromy; y <= toy; y++)
		{
			// Is this a single row?
			if (toy == fromy)
			{
				// Single Row
				PaintCellTopEdge(x, y, RECTANGLE_EDGE);
				PaintCellBottomEdge(x, y, RECTANGLE_EDGE);
			} else 
			// Multiple Rows
			// Top, Middle or Bottom
			if (y == fromy)
			{
				// Top Row
				PaintCellTopEdge(x, y, RECTANGLE_EDGE);
				PaintCellBottomEdge(x, y, INTERIOR_EDGE);
			}
			else if (y < toy) {
				// Middle Rows
				PaintCellTopEdge(x, y, INTERIOR_EDGE);
				PaintCellBottomEdge(x, y, INTERIOR_EDGE);
			}
			else {
				// Bottom Row
				PaintCellTopEdge(x, y, INTERIOR_EDGE);
				PaintCellBottomEdge(x, y, RECTANGLE_EDGE);
			}

			
			// Is this a single column ?
			if (fromx == tox)
			{
				PaintCellLeftEdge(x, y, RECTANGLE_EDGE);
				PaintCellRightEdge(x, y, RECTANGLE_EDGE);
			} else 
			// Multiple columns
			// Left, Middle, or Right
			if (x == fromx)
			{
				// Left Column
				PaintCellLeftEdge(x, y, RECTANGLE_EDGE);
				PaintCellRightEdge(x, y, INTERIOR_EDGE);
			}
			else if (x < tox) {
				// Middle Column
				PaintCellLeftEdge(x, y, INTERIOR_EDGE);
				PaintCellRightEdge(x, y, INTERIOR_EDGE);
			}
			else {
				// Right Column
				PaintCellLeftEdge(x, y, INTERIOR_EDGE);
				PaintCellRightEdge(x, y, RECTANGLE_EDGE);
			}

			/////////////////////////////////////////////////
			// Cell Interior
			PaintCellInterior(x, y, INTERIOR_USED);
		}
	}
}

void CPictureControlTestDlg::PaintCellLeftEdge(int cellx, int celly, COLORREF clr)
{
	int x = cellx * cell_width;
	int y = celly * cell_height;
	AddCellRect(x, y, LINE_WIDTH, cell_height - LINE_WIDTH, clr);
}

void CPictureControlTestDlg::PaintCellRightEdge(int cellx, int celly, COLORREF clr)
{
	int x = (cellx+1) * cell_width - LINE_WIDTH;
	int y = celly * cell_height;
	AddCellRect(x, y, LINE_WIDTH, cell_height - LINE_WIDTH, clr);
}

void CPictureControlTestDlg::PaintCellTopEdge(int cellx, int celly, COLORREF clr)
{
	int x = cellx * cell_width;
	int y = celly * cell_height;
	AddCellRect(x, y, cell_width, LINE_WIDTH, clr);
}

void CPictureControlTestDlg::PaintCellBottomEdge(int cellx, int celly, COLORREF clr)
{
	int x = cellx * cell_width;;
	int y = (celly+1) * cell_height - LINE_WIDTH;
	AddCellRect(x, y, cell_width, LINE_WIDTH, clr);
}

void CPictureControlTestDlg::PaintCellInterior(int cellx, int celly, COLORREF clr)
{
	int x = cellx * cell_width + LINE_WIDTH;
	int y = celly * cell_height + LINE_WIDTH;
	int wid = cell_width - 2 * LINE_WIDTH;
	int hgt = cell_height - 2 * LINE_WIDTH;
	AddCellRect(x, y, wid, hgt, clr);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPictureControlTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPictureControlTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPictureControlTestDlg::AddRectangle(int fromx, int fromy, int tox, int toy)
{
	int t_fromx = fromx;
	int t_fromy = fromy;
	int t_tox = tox;
	int t_toy = toy;
	if (fromx > tox || fromy > toy)
	{
		//CString str;
		//str.Format(_T("To values must be greater than From values"));
		//MessageBox(str);
		//result = false;
		t_fromx = (tox < fromx) ? tox : fromx;
		t_fromy = (toy < fromy) ? toy : fromy;
		t_tox = (tox > fromx) ? tox : fromx;
		t_toy = (toy > fromy) ? toy : fromy;
	}

	// Check Cells aren't already in a Rectangle
	boolean result = true;
	for (int i = t_fromx; i <= t_tox; i++)
		for (int j = t_fromy; j <= t_toy; j++)
		{
			if (result && cellsFree[i][j] == false)
			{
				CString str;
				str.Format(_T("Cell at x=%d y= %d  is in a Rectangle"), i, j);
				MessageBox(str);
				result = false;
			}
		}

	// Mark Cells Used
	if (result)
	{
		for (int i = t_fromx; i <= t_tox; i++)
			for (int j = t_fromy; j <= t_toy; j++)
			{
				cellsFree[i][j] = false;
			}

		// Paint Cells in Rectangle as Used
		PaintRectangle(t_fromx, t_fromy, t_tox, t_toy);

		m_picture.Invalidate();	// redraw layout

		// Add rectangle to array
		layoutRects.emplace_back(t_fromx, t_fromy, t_tox, t_toy);
	}

	mouseDownX = 0;
	mouseDownY = 0;

	ResetEditControls();
}

void CPictureControlTestDlg::OnBnClickedAddRectangle()
{
	// control notification handler code here
	boolean result = true;

	// Validate Input
	UpdateData();

	int fromx = 0;
	int fromy = 0;
	int tox = 0;
	int toy = 0;;

	// From X
	fromx = CheckParamFromEdit(&m_from_x, _T("From"), _T("X"), cells_horiz);
	if (fromx < 0)
		result = false;

	// From Y
	if (result)
	{
		fromy = CheckParamFromEdit(&m_from_y, _T("From"), _T("Y"), cells_vert);
		if (fromy < 0)
			result = false;
	}

	// ToX
	if (result)
	{
		tox = CheckParamFromEdit(&m_to_x, _T("To"), _T("X"), cells_horiz);
		if (tox < 0)
			result = false;
	}

	// ToY
	if (result)
	{
		toy = CheckParamFromEdit(&m_to_y, _T("To"), _T("Y"), cells_vert);
		if (tox < 0)
			result = false;
	}

	if (result && fromx > tox || fromy > toy)
	{
		CString str;
		str.Format(_T("To values must be greater than From values"));
		MessageBox(str);
		result = false;
	}

	// Check Cells aren't already in a Rectangle
	if (result)
	{
		for (int i = fromx; i <= tox; i++)
			for (int j = fromy; j <= toy; j++)
			{
				if (cellsFree[i][j] == false)
				{
					CString str;
					str.Format(_T("Cell at x=%d y= %d  is in a Rectangle"), i, j);
					MessageBox(str);
					result = false;
				}
			}
	}

	// Mark Cells Used
	if (result)
	{
		for (int i = fromx; i <= tox; i++)
			for (int j = fromy; j <= toy; j++)
			{
				cellsFree[i][j] = false;
			}

		// Paint Cells in Rectangle as Used
		PaintRectangle(fromx, fromy, tox, toy);

		m_picture.Invalidate();	// redraw layout

		// Add rectangle to array
		layoutRects.emplace_back(fromx, fromy, tox, toy);
	}

	ResetEditControls();
}

void CPictureControlTestDlg::ResetEditControls()
{
	// Reset Edit Controls
	m_from_x.SetSel(0, -1);
	m_from_x.Clear();
	m_from_y.SetSel(0, -1);
	m_from_y.Clear();
	m_to_x.SetSel(0, -1);
	m_to_x.Clear();
	m_to_y.SetSel(0, -1);
	m_to_y.Clear();
}

void CPictureControlTestDlg::EnableEditCtrls(boolean fromx, boolean fromy, boolean tox, boolean toy)
{
	m_from_x.EnableWindow(fromx);
	m_from_y.EnableWindow(fromy);
	m_to_x.EnableWindow(tox);
	m_to_y.EnableWindow(toy);
}

bool CPictureControlTestDlg::IsLayoutComplete()
{
	for (int i = 0; i < cells_horiz; i++)
		for (int j = 0; j < cells_vert; j++)
		{
			if (cellsFree[i][j])
			{
				return false;
			}
		}
	return true;
}

bool CPictureControlTestDlg::DisplayRectangleList()
{
	bool result = false;
	char buffer[100];
	if (layoutRects.empty())
	{
		sprintf_s(buffer, "Layout Empty\n");
		OutputDebugStringA(buffer);
	}
	else
	{
		if (!IsLayoutComplete())
		{
			sprintf_s(buffer, "Layout Incomplete\n");
			OutputDebugStringA(buffer);
		}

		for (int i = 0; i < (int)layoutRects.size(); i++)
		{
			LayoutRect lr = layoutRects[i];
			sprintf_s(buffer, "Rectangle From XY( %3d, %3d)  ToXY( %3d, %3d )\n", 
				lr.m_fromx, lr.m_fromy, lr.m_tox, lr.m_toy);
			OutputDebugStringA(buffer);
			result = true;
		}
	}
	return result;
}

int CPictureControlTestDlg::CheckParamFromEdit(CEdit *edit, CString toOrFrom, CString xOrY, int max)
{
	char buf[10];
	CString str;
	int value = 0;
	if (0 == edit->GetLine(10, (LPTSTR)buf))
	{
		// **BUG** FIXME
		// This is a work around to a bug that appears in the Release build
		// Everything is OK in Debug mode, but in Release build edirt control appears empty
		// Maybe will remove edit controls completely
		return 0;
		//str.Format(_T("%s %s empty"), toOrFrom, xOrY);
		//MessageBox(str);
		//return -1;
	}
	value = atoi(buf);
	if (value >= max)
	{
		str.Format(_T("%s %s= %d  Must be < %d"), toOrFrom, xOrY, value, max);
		MessageBox(str);
		return -1;
	}
	return value;
}

void CPictureControlTestDlg::AddCellRect(int x, int y, int wid, int hgt, COLORREF clr)
{
	m_picture.AddCellRect(x, y, wid, hgt, clr);
}

void CPictureControlTestDlg::OnBnClickedReset()
{
	// TODO: Add your control notification handler code here
	Reset();
}

// Reset Rectangle array in Picture control
// Repaint Picture Control
// Reset cells Free array
void CPictureControlTestDlg::Reset()
{
	char buffer[100];
	sprintf_s(buffer, "\n*** R E S E T ****\n\n" );
	OutputDebugStringA(buffer);

	m_picture.ClearCellRects();
	m_picture.ExOnPaint();		// Tell Grid to repaint itself
	layoutRects.clear();

	SetUpLayout();

	SetCellsFree(true);		// set cellsFree to true
	
	ResetEditControls();
	m_picture.Invalidate();
}


void CPictureControlTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( DisplayRectangleList() )
	{ 
		str.Format(_T("Save Rectangle List?"));
		if (IDYES == MessageBox(str, _T(""), MB_YESNO | MB_ICONSTOP))
		{
			SaveRectanglesToFile();
		}
		CDialogEx::OnOK();
	}
	else
	{
		str.Format(_T("Exit "));
		if ( IDYES == MessageBox( str,_T(""), MB_YESNO ) ) //| MB_ICONSTOP) )
			CDialogEx::OnOK();
	}
}

boolean CPictureControlTestDlg::SaveRectanglesToFile()
{
	CFileDialog dlgFile( FALSE, CString(".txt"), NULL, OFN_OVERWRITEPROMPT , CString("Text Files (*.txt)|*.txt|"));

	if ( dlgFile.DoModal() == IDOK)
	{ 
		fileName = dlgFile.GetPathName( );	// fileName is global
		return SaveRectanglesToFile(layoutRects, fileName);
	}
	return false;
}

boolean CPictureControlTestDlg::SaveRectanglesToFile(std::vector<LayoutRect> list, CString filenm)
{
	CStdioFile file;
	UINT flags = CFile::modeCreate | CFile::modeWrite;// | CFile::typeText;

	CFileException e;
	try
	{
		file.Open(filenm, flags, &e);
	}
	catch (...)
	{
		CString err;
		err.GetBuffer(100);
		e.GetErrorMessage(err.GetBuffer(), 100);
	}

	//// Ready to write
	char str[200];

	// Safety Value - check this when we read it to avoid reading an inappropriate file
	sprintf_s(str, "%d\n\0", SAFETY_VALUE);
	file.Write((LPCTSTR)str, strlen(str));

	// Width and Height of cells in layout
	//sprintf(str, "# Size - Width Height\n\0");
	strcpy_s(str, "# Size - Width Height\n\0");
	file.Write((LPCTSTR)str, strlen(str));

	sprintf_s(str, "%d %d\n\0", cells_horiz, cells_vert);
	file.Write((LPTSTR)str, strlen(str));

	// Number of Rectangles
	strcpy_s(str, "# Number of Rectangles\n\0");
	file.Write((LPCTSTR)str, strlen(str));

	int nrects = (int)list.size();
	sprintf_s(str, "%d\n\0", nrects);
	file.Write((LPTSTR)str, strlen(str));

	strcpy_s(str, "# Rectangles  FromX  FromY  ToX  ToY\n\0");
	file.Write((LPCTSTR)str, strlen(str));

	for (int i = 0; i < nrects; i++)
	{
		//CString str;
		char str[100];
		LayoutRect r = list[i];
		sprintf_s(str, "%d %d %d %d\n\0", r.m_fromx, r.m_fromy, r.m_tox, r.m_toy);
		file.Write((LPTSTR)str, strlen(str));
	}

	file.Close();
	return true;
}


void CPictureControlTestDlg::OnBnClickedButtonSave()
{
	SaveReset(false);
}

void CPictureControlTestDlg::OnBnClickedButtonSaveReset()
{
	SaveReset(true);
}

void CPictureControlTestDlg::SaveReset(boolean reset)
{
	if (!IsLayoutComplete())
	{
		AfxMessageBox(_T("Incomplete Layout"));
		return;
	}
	if (SaveRectanglesToFile())
	{
		if ( reset ) Reset();
		SaveLayoutImage();
	}
}

boolean CPictureControlTestDlg::ReadRectsFromFile()
{
	CString fileName;
	wchar_t* p = fileName.GetBuffer(MAX_PATH + 1);
	CFileDialog dlgFile(TRUE);

	dlgFile.DoModal();
	//fileName.ReleaseBuffer();
	fileName = dlgFile.GetPathName();

	CStdioFile file;
	UINT flags = CFile::modeRead;// | CFile::typeText;

	CFileException e;
	try
	{
		file.Open(fileName, flags, &e);
	}
	catch (...)
	{
		CString err;
		err.GetBuffer(100);
		e.GetErrorMessage(err.GetBuffer(), 100);
	}

	///////////////////////////////////////////////////////////
	//// Ready to Read

	// Safety Value - check this when we read it to avoid reading an inappropriate file
	GetNextLine(&file);
	
	int safety;
	sscanf_s(buffer, "%d", &safety);

	 // Debug trace output
	char prtBuf[100];
	sprintf_s(prtBuf, "Safety %d\n", safety);
	OutputDebugStringA(prtBuf);

	if (safety != SAFETY_VALUE)
	{
		MessageBox((LPCTSTR) "error Reading file");
		return false;
	}

	///////////////////////////////////////////////////////
	// Width and Height of cells in layout
	GetNextLine(&file);
	//ConvertCString(readLine, buffer);
	int width, height;
	sscanf_s(buffer, "%d %d", &width, &height );

	sprintf_s(prtBuf, "Width %d  Height%d\n", width, height);
	OutputDebugStringA(prtBuf);

	// CHeck width and height (??)
	if (width != cells_horiz || height != cells_vert)
	{
		CString str;
		str.Format(_T(" Grid is Wd x Ht = (%d,%d), Layout needs (%d, %d"),
			cells_horiz, cells_vert, width, height);
		MessageBox(str);
		return false;
	}
	cells_horiz = width;
	cells_vert = height;

	////////////////////////////////////////////////////
	// Number of Rectangles
	GetNextLine(&file);
	//ConvertCString(readLine, buffer);
	int nrects;
	sscanf_s(buffer, "%d", &nrects);

	sprintf_s(prtBuf, "Number Rectangles %d\n", nrects );
	OutputDebugStringA(prtBuf);

	///////////////////////////////////////////////////
	// Read Rectangles
	for (int i = 0; i < nrects; i++)
	{
		int fromx, fromy, tox, toy;
		GetNextLine(&file);
		//ConvertCString(readLine, buffer);
		sscanf_s(buffer, "%d %d %d %d", &fromx, &fromy, &tox, &toy);
		// Add rectangle to array
		layoutRects.emplace_back(fromx, fromy, tox, toy);

		sprintf_s(prtBuf, "From XY (%d, %d)  ToXY (%d, %d)\n", fromx, fromy, tox, toy);
		OutputDebugStringA(prtBuf);
	}

	file.Close();
	return true;
}

// There should be an easier way to convert from CString to something 
// that sscanf likes, ie LPCTSTR -- but this worked
void CPictureControlTestDlg::ConvertCString(CString s, char chrs[])
{
	for (int i = 0; i < s.GetLength(); i++)
	{
		chrs[i] = (char) s.GetAt(i);
	}
	chrs[s.GetLength()] = 0;
}

void CPictureControlTestDlg::ConvertCString(CString s, TCHAR chrs[])
{
	for (int i = 0; i < s.GetLength(); i++)
	{
		chrs[i] = (char)s.GetAt(i);
	}
	chrs[s.GetLength()] = 0;
}

//  The line read is put into buffer, a global variable in this class
//	Not the best way of doing this since it is a hidden side effect.
//	Tried several other explicit ways of doing this, 
//	but kept having character set conversion problems
void CPictureControlTestDlg::GetNextLine(CStdioFile *file)
{
	const char pound = *"#";

	while (file->ReadString( readLine))
	{
		if (readLine.GetAt(0) != pound)
		{
			ConvertCString(readLine, buffer);
			return;
		}
	}
}

void CPictureControlTestDlg::OnBnClickedButtonRead()
{
	Reset();
	ReadRectsFromFile();
	SetNewGridLayout();
}

// Called after a new set of layoutRects are put into 
//		std::vector<LayoutRect> layoutRects;
void CPictureControlTestDlg::SetNewGridLayout()
{
	// Mark Cells Used
	SetCellsFree(true);
	ResetEditControls();
	PaintRects();
}

void CPictureControlTestDlg::SetNewGridLayout(std::vector<LayoutRect> list)
{
	// Mark Cells Used
	SetCellsFree(true);
	ResetEditControls();
	PaintRects(list);
}

void CPictureControlTestDlg::PaintRects()
{
	PaintRects(layoutRects);
}

void CPictureControlTestDlg::PaintRects(std::vector<LayoutRect> list)
{
	for (int i = 0; i < (int)list.size(); i++)
	{
		LayoutRect r = list[i];
		// Paint Cells in Rectangle as Used
		PaintRectangle(r.m_fromx, r.m_fromy, r.m_tox, r.m_toy);
	}
	m_picture.Invalidate();
}

void CPictureControlTestDlg::SetCellsFree(boolean trueOrFalse)
{
	// Mark Cells 
	for (int i = 0; i < max_cells_horiz; i++)
		for (int j = 0; j < max_cells_vert; j++)
		{
			cellsFree[i][j] = trueOrFalse;
		}
}


void CPictureControlTestDlg::OnBnClickedButtonCreategrid()
{
	// Read Cells Horiz and Vert and Create Grid
	cells_vert  = CheckParamFromEdit(&m_cells_vert,  _T("Cells"), _T("Vert"), max_cells_vert);
	cells_horiz = CheckParamFromEdit(&m_cells_horiz, _T("Cells"), _T("horiz"), max_cells_horiz);

	SetUpLayout();
	m_picture.Invalidate();
}

boolean CPictureControlTestDlg::CEditIsEmpty(CEdit *ctrl)
{
	CString str;
	ctrl->GetWindowTextW(str);
	return str.IsEmpty();
}

CPoint CPictureControlTestDlg::GetCellXY()
{
	// Read mouse location
	POINT screenPt;
	BOOL result = GetCursorPos(&screenPt);
	//POINT windowPoint;
	RECT r;
	m_picture.GetWindowRect(&r);

	//WindowFromPoint(screenPt);
	//HWND hwnd = m_picture.GetWindow
	//m_picture.ScreenToClient(m_picture.m_hWnd, &windowPoint);
	int rectX = screenPt.x - r.left;
	int rectY = screenPt.y - r.top;

	int cellX = rectX / cell_width;
	int cellY = rectY / cell_height;
	return CPoint(cellX, cellY);
}

void CPictureControlTestDlg::SetEdit( CEdit *edit, int value)
{
	TCHAR str[20];
	swprintf_s(str, _T("%d\0\0"), value);	// was _swprintf
	edit->SetWindowText((LPCTSTR)str);
}

void CPictureControlTestDlg::SaveLayoutImage()
{
	SaveLayoutImage(fileName);	// fileName is a global
}

void CPictureControlTestDlg::SaveLayoutImage( CString fname )
{
	// remove .txt and add .bmp
	CString bmp = _T(".bmp");
	CString szPathName;
	int loc = fname.Find(_T(".txt"));
	if (loc > 0)
	{
		szPathName = fname.Left(fname.GetLength()-4)  + bmp;
	}
	else
	{
		szPathName = fname + bmp;
	}
	
	char chname[100];
	ConvertCString(szPathName, chname);

	//CRect rect;
	//m_picture.GetWindowRect(rect);

	//CDC *dc = m_picture.GetDC();
	//SaveDCToBitmap(chname, rect, dc );

	m_picture.SaveDCToBitmap(chname);
}

int CPictureControlTestDlg::SaveDCToBitmap( const char *pszFile, RECT rc, CDC *dc)
{
	BITMAPFILEHEADER bmfh = { 0 };
	BITMAPINFOHEADER bmih = { 0 };
	BITMAPINFO bi;
	
	HDC hdc1 = *dc;
	HDC hdc2 = CreateCompatibleDC(hdc1);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;

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

	HANDLE fileHandle = CreateFileA( pszFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
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

void CPictureControlTestDlg::OnBnClickedButtonRotateLeft()
{
	std::vector<LayoutRect> newLayoutRects = RotateLeft(layoutRects);
	VerifyLayoutList(newLayoutRects);
	Reset();		// Clears layoutRects

	// Copy newLayouts to layoutRects
	for (int i = 0; i < (int)newLayoutRects.size(); i++)
	{
		layoutRects.emplace_back(newLayoutRects[i]);
	}
	SetNewGridLayout();
}

void CPictureControlTestDlg::OnBnClickedButtonReflectVert()
{
	std::vector<LayoutRect> newLayoutRects = ReflectVert(layoutRects);
	VerifyLayoutList(newLayoutRects);
	Reset();		// Clears layoutRects

	// Copy newLayouts to layoutRects
	for (int i = 0; i < (int)newLayoutRects.size(); i++)
	{
		layoutRects.emplace_back(newLayoutRects[i]);
	}
	SetNewGridLayout();
}

void CPictureControlTestDlg::OnBnClickedButtonReflectHoriz()
{
	std::vector<LayoutRect> newLayoutRects = ReflectHoriz(layoutRects);
	Reset();		// Clears layoutRects

	// Copy newLayouts to layoutRects
	for (int i = 0; i < (int)newLayoutRects.size(); i++)
	{
		layoutRects.emplace_back(newLayoutRects[i]);
	}
	SetNewGridLayout();
}

std::vector<LayoutRect> CPictureControlTestDlg::ReflectHoriz(std::vector<LayoutRect> list)
{
	int n = cells_vert - 1;	// negative coordinates are subtracted from this

		// Copy Rectangle list and change coordinates
	std::vector<LayoutRect> newLayoutRects;
	for (int i = 0; i < (int)list.size(); i++)
	{
		LayoutRect oldR = list.at(i);
		int newFromX = oldR.m_fromx;
		int newFromY = n - oldR.m_fromy;
		int newToX = oldR.m_tox;
		int newToY = n - oldR.m_toy;

		LayoutRect newR(newFromX, newFromY, newToX, newToY);
		//LayoutRect newR = LayoutRect::CorrectXY(newFromX, newFromY, newToX, newToY);

		newLayoutRects.emplace_back(newR);
	}
	VerifyLayoutList(newLayoutRects);
	return newLayoutRects;
}

std::vector<LayoutRect> CPictureControlTestDlg::ReflectVert(std::vector<LayoutRect> list)
{
	int n = cells_vert - 1;	// negative coordinates are subtracted from this

	// Copy Rectangle list and change coordinates
	std::vector<LayoutRect> newLayoutRects;
	for (int i = 0; i < (int)list.size(); i++)
	{
		LayoutRect oldR = list.at(i);
		int newFromX = n - oldR.m_fromx;
		int newFromY = oldR.m_fromy;
		int newToX = n - oldR.m_tox;
		int newToY = oldR.m_toy;

		LayoutRect newR(newFromX, newFromY, newToX, newToY);
		//LayoutRect newR = LayoutRect::CorrectXY(newFromX, newFromY, newToX, newToY);

		newLayoutRects.emplace_back(newR);
	}
	VerifyLayoutList(newLayoutRects);
	return newLayoutRects;
}

std::vector<LayoutRect> CPictureControlTestDlg::RotateLeft(std::vector<LayoutRect> list)
{
	// TODO: Add your control notification handler code here
	if (cells_vert != cells_horiz)
	{
		AfxMessageBox(_T("Can't Rotate if not Square"), MB_OK);
		return (std::vector<LayoutRect>) NULL;
	}
	int n = cells_vert - 1;	// negative coordinates are subtracted from this

	// Copy Rectangle list and change coordinates
	std::vector<LayoutRect> newLayoutRects;
	for (int i = 0; i < (int)list.size(); i++)
	{
		LayoutRect oldR = list.at(i);
		int newFromX = oldR.m_fromy;
		int newFromY = n - oldR.m_fromx;
		int newToX = oldR.m_toy;
		int newToY = n - oldR.m_tox;

		LayoutRect newR(newFromX, newFromY, newToX, newToY);
		//LayoutRect newR = LayoutRect::CorrectXY(newFromX, newFromY, newToX, newToY);

		newLayoutRects.emplace_back(newR);
	}
	VerifyLayoutList(newLayoutRects);
	return newLayoutRects;
}

void CPictureControlTestDlg::OnBnClickedButtonDeleteLast()
{
	// TODO: Add your control notification handler code here
	if (layoutRects.empty())
		return;

	LayoutRect last = layoutRects.back();
	layoutRects.pop_back();

	int fromx = last.m_fromx;
	int fromy = last.m_fromy;
	int tox = last.m_tox;
	int toy = last.m_toy;
	for (int i = fromx; i <= tox; i++)
		for (int j = fromy; j <= toy; j++)
		{
			cellsFree[i][j] = true;
		}

	// Clear Grid
	m_picture.ClearCellRects();
	m_picture.ExOnPaint();

	SetUpLayout();

	// Paint Cells in Rectangle as Used
	PaintRects();
}

// Check each Rect in a list is properly oriented
// ie upper left corner is (FromX, FromY)
void CPictureControlTestDlg::VerifyLayoutList(std::vector<LayoutRect> &list)
{
	//vector<LayoutRect>::iterator it;  // declare an iterator to a vector of LayoutRect
	//for (it = list.begin(); it != list.end(); it++)
	//PrintLayoutList(list);
	for (int i=0; i<(int)list.size(); i++)
	{
		LayoutRect r = list.at(i);
		r.CorrectXY();
		list[i] = r;
	}
	SortLayoutList(list);
	//PrintLayoutList(list);
}

void CPictureControlTestDlg::PrintLayoutList(std::vector<LayoutRect> list)
{
	for (int i = 0; i < (int)list.size(); i++)
	{
		char prtBuf[100];

		LayoutRect r = list.at(i);
		sprintf_s(prtBuf, "**** From(%d, %d) To(%d, %d)\n", r.m_fromx, r.m_fromy, r.m_tox, r.m_toy);
		OutputDebugStringA(prtBuf);
	}
}

void CPictureControlTestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//
	// If user use mouse to define rect - clear edit controls
	//
	// If mouse is not in Grid, do nothing.
	//
	// Capture FromX and FromY and set Edit Controls

	if (IsMouseInGrid())
	{
		// if Cell is in use, ignore click
		CPoint cellXY = GetCellXY();
		if (cellsFree[cellXY.x][cellXY.y])
		{
			ResetEditControls();
			SetEdit(&m_from_x, cellXY.x);
			SetEdit(&m_from_y, cellXY.y);
			mouseDownX = cellXY.x;
			mouseDownY = cellXY.y;
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CPictureControlTestDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Add your message handler code here and/or call default
	//
	// If mouse is not in Grid, do nothing.
	// When mouse went down, it may not have been in Grid.
	// So if From edit controls are not set, ignore
	//

	if (IsMouseInGrid() && !CEditIsEmpty(&m_from_x) && !CEditIsEmpty(&m_from_y) )
	{
		CPoint cellXY = GetCellXY();
		// if Cell is in use, ignore click
		if (cellsFree[cellXY.x][cellXY.y])
		{
			// Capture ToX and ToY and set Edit Controls
			// Set TO edit controls
			SetEdit(&m_to_x, cellXY.x);
			SetEdit(&m_to_y, cellXY.y);

			// Now can add rectangle
			//OnBnClickedAddRectangle();
			AddRectangle(mouseDownX, mouseDownY, cellXY.x, cellXY.y);
		} else
		{
			// Clear From Edit controls and start over
			ResetEditControls();
		}
	}
	else
	{
		ResetEditControls();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

boolean CPictureControlTestDlg::IsMouseInGrid()
{
	POINT screenPt;
	BOOL result = GetCursorPos(&screenPt);
	if (result)
	{
		RECT r;
		m_picture.GetWindowRect(&r);
		CRect cr(r);
		CPoint cp(screenPt);
		return cr.PtInRect(cp);
	}
	else
		return false;
}


void CPictureControlTestDlg::OnBnClickedButtonSymmetries()
{
	// TODO: Add your control notification handler code here
	if (!IsLayoutComplete())
	{
		AfxMessageBox(_T("Incomplete Layout"));
		return;
	}

	// There are 8 possible symmetries of the layout
	// Some may be identical, so they will not be saved
	//
	// 8 Symmetries are: Original, Rotate Left 90, 180 and 270
	// and reflect each vertically
	std::vector<LayoutRect> rot90  = RotateLeft(layoutRects);
	std::vector<LayoutRect> rot180 = RotateLeft(rot90);
	std::vector<LayoutRect> rot270 = RotateLeft(rot180);

	// Reflect original and rotated ones
	std::vector<LayoutRect> reflect    = ReflectVert(layoutRects);
	std::vector<LayoutRect> reflect90  = ReflectVert(rot90);
	std::vector<LayoutRect> reflect180 = ReflectVert(rot180);
	std::vector<LayoutRect> reflect270 = ReflectVert(rot270);

	// Create indicators whether each is different from one already tested
	boolean keepRot90 = true;
	boolean keepRot180 = true;
	boolean keepRot270 = true;
	boolean keepReflect = true;
	boolean keepReflect90 = true;
	boolean keepReflect180 = true;
	boolean keepReflect270 = true;

	// Now compare each pair to see if they are equivalent
	// (There may be a more efficient way of doing this series of comparisons)
	if (EqualLayouts(layoutRects, rot90))
		keepRot90 = false;
	if (EqualLayouts(layoutRects, rot180))
		keepRot180 = false;
	if (EqualLayouts(layoutRects, rot270))
		keepRot270 = false;
	if (EqualLayouts(layoutRects, reflect))
		keepReflect = false;
	if (EqualLayouts(layoutRects, reflect90))
		keepReflect90 = false;
	if (EqualLayouts(layoutRects, reflect180))
		keepReflect180 = false;
	if (EqualLayouts(layoutRects, reflect270))
		keepReflect270 = false;

	if (keepRot90)
	{
		if ( keepRot180 && EqualLayouts(rot90, rot180) )
			keepRot180 = false;
		if ( keepRot270 && EqualLayouts(rot90, rot270))
			keepRot270 = false;
		if ( keepReflect && EqualLayouts(rot90, reflect))
			keepReflect = false;
		if ( keepReflect90 && EqualLayouts(rot90, reflect90))
			keepReflect90 = false;
		if ( keepReflect180 && EqualLayouts(rot90, reflect180))
			keepReflect180 = false;
		if ( keepReflect270 && EqualLayouts(rot90, reflect270))
			keepReflect270 = false;
	}

	if (keepRot180)
	{
		if (keepRot270 && EqualLayouts(rot180, rot270))
			keepRot270 = false;
		if (keepReflect && EqualLayouts(rot180, reflect))
			keepReflect = false;
		if (keepReflect90 && EqualLayouts(rot180, reflect90))
			keepReflect90 = false;
		if (keepReflect180 && EqualLayouts(rot180, reflect180))
			keepReflect180 = false;
		if (keepReflect270 && EqualLayouts(rot180, reflect270))
			keepReflect270 = false;
	}

	if (keepRot270)
	{
		if (keepReflect && EqualLayouts(rot270, reflect))
			keepReflect = false;
		if (keepReflect90 && EqualLayouts(rot270, reflect90))
			keepReflect90 = false;
		if (keepReflect180 && EqualLayouts(rot270, reflect180))
			keepReflect180 = false;
		if (keepReflect270 && EqualLayouts(rot270, reflect270))
			keepReflect270 = false;
	}

	if (keepReflect)
	{
		if (keepReflect90 && EqualLayouts(reflect, reflect90))
			keepReflect90 = false;
		if (keepReflect180 && EqualLayouts(reflect, reflect180))
			keepReflect180 = false;
		if (keepReflect270 && EqualLayouts(reflect, reflect270))
			keepReflect270 = false;
	}

	if (keepReflect90)
	{
		if (keepReflect180 && EqualLayouts(reflect90, reflect180))
			keepReflect180 = false;
		if (keepReflect270 && EqualLayouts(reflect90, reflect270))
			keepReflect270 = false;
	}

	if (keepReflect180)
	{
		if (keepReflect270 && EqualLayouts(reflect180, reflect270))
			keepReflect270 = false;
	}


	// Get the root file name
	CFileDialog dlgFile(FALSE, CString(".txt"), NULL, OFN_OVERWRITEPROMPT, CString("Text Files (*.txt)|*.txt|"));
	if (dlgFile.DoModal() == IDOK)
	{

		fileName = dlgFile.GetPathName();

		// Now that we have compared them all pairwise
		// keep the ones that are different
		// Save layout directly created by user
		if (SaveRectanglesToFile(layoutRects, fileName))
		{
			SaveLayoutImage(fileName);
		}
		else
			return;

		CString rootFileName = dlgFile.GetFileTitle();
		CString filePath = dlgFile.GetFolderPath();
		CString rootFullFileName = filePath + "\\" + rootFileName;

		////////////////////////////////////////////////////////////////////////////////
		// Check to see that all file names are avilable - 
		// ie no files exist with the file names that will be created for the symmetrical layouts
		//
		// Create CStrings for file names to make code more readable
		CString fn_rot90      = rootFullFileName + _T("-rot90.txt");
		CString fn_rot180     = rootFullFileName + _T("-rot180.txt");
		CString fn_rot270     = rootFullFileName + _T("-rot270.txt");
		CString fn_reflect    = rootFullFileName + _T("-reflect.txt");
		CString fn_reflect90  = rootFullFileName + _T("-reflect90.txt");
		CString fn_reflect180 = rootFullFileName + _T("-reflect180.txt");
		CString fn_reflect270 = rootFullFileName + _T("-reflect270.txt");

		///////////////////////////////////////////////////////////////////////
		// If any file names are not available, ask user for a new name
		boolean doFileSave = true;
		if (keepRot90 && !FileNameIsAvailable(fn_rot90) )
		{
			doFileSave = false;
		}
		if (doFileSave && keepRot180 && !FileNameIsAvailable(fn_rot180))
		{
			doFileSave = false;
		}
		if (doFileSave && keepRot270 && !FileNameIsAvailable(fn_rot270))
		{
			doFileSave = false;
		}

		if (doFileSave && keepReflect && !FileNameIsAvailable(fn_reflect))
		{
			doFileSave = false;
		}
		if (doFileSave && keepReflect90 && !FileNameIsAvailable(fn_reflect90))
		{
			doFileSave = false;
		}
		if (doFileSave && keepReflect180 && !FileNameIsAvailable(fn_reflect180))
		{
			doFileSave = false;
		}
		if (doFileSave && keepReflect270 && !FileNameIsAvailable(fn_reflect270))
		{
			doFileSave = false;
		}

		/////////////////////////////////////////////////////////////////////////
		if (doFileSave)
		{
			// OK. All needed File names are available -- Save unique symmetry files
			if (keepRot90)
			{
				SaveSymmetricalLayout(rot90, fn_rot90);
			}
			if (keepRot180)
			{
				SaveSymmetricalLayout(rot180, fn_rot180);
			}
			if (keepRot270)
			{
				SaveSymmetricalLayout(rot270, fn_rot270);
			}

			if (keepReflect)
			{
				SaveSymmetricalLayout(reflect, fn_reflect);
			}
			if (keepReflect90)
			{
				SaveSymmetricalLayout(reflect90, fn_reflect90);
			}
			if (keepReflect180)
			{
				SaveSymmetricalLayout(reflect180, fn_reflect180);
			}
			if (keepReflect270)
			{
				SaveSymmetricalLayout(reflect270, fn_reflect270);
			}
			Reset();
		}
		else
		{
			AfxMessageBox(_T("Not saved.\nSymmetry file(s) already exist(s).\nChoose different name."));
		}
	}

}

// Return true if file was not found
boolean CPictureControlTestDlg::FileNameIsAvailable(CString filename)
{
	CFileFind ff;
	int res = ff.FindFile((LPCTSTR)filename);
	return res == 0;
}

void CPictureControlTestDlg::SaveSymmetricalLayout(std::vector<LayoutRect> &list, CString filename)
{
	Reset();
	SetNewGridLayout(list);
	SaveRectanglesToFile(list, filename);
	m_picture.ExOnPaint();
	SaveLayoutImage(filename);
}

void CPictureControlTestDlg::SortLayoutList(std::vector<LayoutRect> &list)
{
	std::sort(list.begin(), list.end());
}

// Assumes list is sorted and each layout Rect has FromXY as upper left corner
boolean CPictureControlTestDlg::EqualLayouts(std::vector<LayoutRect> list1, std::vector<LayoutRect> list2)
{
	// If not same size then false
	if (list1.size() != list2.size())
		return false;

	VerifyLayoutList(list1);
	VerifyLayoutList(list2);

	// If any are different, then false
	for (int i = 0; i < (int)list1.size(); i++)
	{
		if (!(list1[i] == list2[i]))
			return false;
	}

	// All good
	return true;
}
