// CheckerBoardImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckerBoard.h"
#include "TriangleBoard.h"
#include "EllipseBoard.h"
#include "CheckerBoardImage.h"
#include "BrokenSqr.h"
#include "SquareWithHole.h"
#include "Quadrilateral.h"
//#include "CheckerBoardImage.rc"
#include "CheckerBoardImageDlg.h"
#include "afxdialogex.h"
#include <string>
#include <iomanip>
#include <sys/types.h> 
#include <sys/timeb.h>
#include "afxdlgs.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define TRACE 0


// CCheckerBoardImageDlg dialog
char bfr[200];
#define O OutputDebugStringA
#define TT(s,n) sprintf_s(bfr, "%-20s   0x%08x  %5d\n", s, n, n);

CCheckerBoardImageDlg::CCheckerBoardImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHECKERBOARDIMAGE_DIALOG, pParent)
	, m_layerMode(0)
	, m_shape(0)
	, m_refreshRandom(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckerBoardImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH, m_width);
	DDX_Text(pDX, IDC_HEIGHT, m_height);

	DDX_Control(pDX, IDC_COMBO_ROW0, m_comboRow[0]);
	DDX_Control(pDX, IDC_COMBO_ROW1, m_comboRow[1]);
	DDX_Control(pDX, IDC_COMBO_ROW2, m_comboRow[2]);
	DDX_Control(pDX, IDC_COMBO_ROW3, m_comboRow[3]);
	DDX_Control(pDX, IDC_COMBO_ROW4, m_comboRow[4]);
	DDX_Control(pDX, IDC_COMBO_ROW5, m_comboRow[5]);
	DDX_Control(pDX, IDC_COMBO_ROW6, m_comboRow[6]);
	DDX_Control(pDX, IDC_COMBO_ROW7, m_comboRow[7]);

	DDX_Control(pDX, IDC_COMBO_COL0, m_comboCol[0]);
	DDX_Control(pDX, IDC_COMBO_COL1, m_comboCol[1]);
	DDX_Control(pDX, IDC_COMBO_COL2, m_comboCol[2]);
	DDX_Control(pDX, IDC_COMBO_COL3, m_comboCol[3]);
	DDX_Control(pDX, IDC_COMBO_COL4, m_comboCol[4]);
	DDX_Control(pDX, IDC_COMBO_COL5, m_comboCol[5]);
	DDX_Control(pDX, IDC_COMBO_COL6, m_comboCol[6]);
	DDX_Control(pDX, IDC_COMBO_COL7, m_comboCol[7]);


	DDX_Check(pDX, IDC_CHECK_RANDOM_SQUARE, m_random);
	DDX_Text(pDX, IDC_NUMBER_OF_IMAGES, m_numberOfImages);
	DDV_MinMaxInt(pDX, m_numberOfImages, 1, 100);

	DDX_Radio(pDX, IDC_RADIO_1, m_layerMode);

	DDX_Text(pDX, IDC_EDIT_PAIR_0_L, layerPairValues[0][0]);
	DDV_MinMaxInt(pDX, layerPairValues[0][0], 0, 7);
	//DDX_Text(pDX, IDC_EDIT_PAIR_0_R, layerPairValues[0][1]);
	//DDV_MinMaxInt(pDX, layerPairValues[0][1], 0, 7);

	DDX_Text(pDX, IDC_EDIT_PAIR_1_L, layerPairValues[1][0]);
	DDV_MinMaxInt(pDX, layerPairValues[1][0], 0, 7);
	//DDX_Text(pDX, IDC_EDIT_PAIR_1_R, layerPairValues[1][1]);
	//DDV_MinMaxInt(pDX, layerPairValues[1][1], 0, 7);

	DDX_Text(pDX, IDC_EDIT_PAIR_2_L, layerPairValues[2][0]);
	DDV_MinMaxInt(pDX, layerPairValues[2][0], 0, 7);
	//DDX_Text(pDX, IDC_EDIT_PAIR_2_R, layerPairValues[2][1]);
	//DDV_MinMaxInt(pDX, layerPairValues[2][1], 0, 7);

	DDX_Text(pDX, IDC_EDIT_PAIR_3_L, layerPairValues[3][0]);
	DDV_MinMaxInt(pDX, layerPairValues[3][0], 0, 7);
	//DDX_Text(pDX, IDC_EDIT_PAIR_3_R, layerPairValues[3][1]);
	//DDV_MinMaxInt(pDX, layerPairValues[3][1], 0, 7);

	DDX_Text(pDX, IDC_EDIT_PAIR_4_L, layerPairValues[4][0]);
	DDV_MinMaxInt(pDX, layerPairValues[4][0], 0, 7);
	//DDX_Text(pDX, IDC_EDIT_PAIR_4_R, layerPairValues[4][1]);
	//DDV_MinMaxInt(pDX, layerPairValues[4][1], 0, 7);

	DDX_Text(pDX, IDC_EDIT_PAIR_5_L, layerPairValues[5][0]);
	DDV_MinMaxInt(pDX, layerPairValues[5][0], 0, 7);
	//DDX_Text(pDX, IDC_EDIT_PAIR_5_R, layerPairValues[5][1]);
	//DDV_MinMaxInt(pDX, layerPairValues[5][1], 0, 7);

	DDX_Text(pDX, IDC_EDIT_PAIR_6_L, layerPairValues[6][0]);
	DDV_MinMaxInt(pDX, layerPairValues[6][0], 0, 7);
	//DDX_Text(pDX, IDC_EDIT_PAIR_6_R, layerPairValues[6][1]);
	//DDV_MinMaxInt(pDX, layerPairValues[6][1], 0, 7);

	DDX_Text(pDX, IDC_EDIT_PAIR_7_L, layerPairValues[7][0]);
	DDV_MinMaxInt(pDX, layerPairValues[7][0], 0, 7);
	//DDX_Text(pDX, IDC_EDIT_PAIR_7_R, layerPairValues[7][1]);
	//DDV_MinMaxInt(pDX, layerPairValues[7][1], 0, 7);
	DDX_Control(pDX, IDC_STATIC_LAYER_PAIRS_BOX, m_layerPairBox);

	DDX_Control(pDX, IDC_EDIT_PAIR_0_L, m_layerPair[0][0]);
	DDX_Control(pDX, IDC_EDIT_PAIR_1_L, m_layerPair[1][0]);
	DDX_Control(pDX, IDC_EDIT_PAIR_2_L, m_layerPair[2][0]);
	DDX_Control(pDX, IDC_EDIT_PAIR_3_L, m_layerPair[3][0]);
	DDX_Control(pDX, IDC_EDIT_PAIR_4_L, m_layerPair[4][0]);
	DDX_Control(pDX, IDC_EDIT_PAIR_5_L, m_layerPair[5][0]);
	DDX_Control(pDX, IDC_EDIT_PAIR_6_L, m_layerPair[6][0]);
	DDX_Control(pDX, IDC_EDIT_PAIR_7_L, m_layerPair[7][0]);

	//DDX_Control(pDX, IDC_EDIT_PAIR_0_R, m_layerPair[0][1]);
	//DDX_Control(pDX, IDC_EDIT_PAIR_1_R, m_layerPair[1][1]);
	//DDX_Control(pDX, IDC_EDIT_PAIR_2_R, m_layerPair[2][1]);
	//DDX_Control(pDX, IDC_EDIT_PAIR_3_R, m_layerPair[3][1]);
	//DDX_Control(pDX, IDC_EDIT_PAIR_4_R, m_layerPair[4][1]);
	//DDX_Control(pDX, IDC_EDIT_PAIR_5_R, m_layerPair[5][1]);
	//DDX_Control(pDX, IDC_EDIT_PAIR_6_R, m_layerPair[6][1]);
	//DDX_Control(pDX, IDC_EDIT_PAIR_7_R, m_layerPair[7][1]);

	DDX_Radio(pDX, IDC_RADIO_RECTANGLE, m_shape);
	DDX_Control(pDX, IDC_REFRESH_RANDOM, m_refreshRandomCtrl);
	DDX_Check(pDX, IDC_REFRESH_RANDOM, m_refreshRandom);
}

BEGIN_MESSAGE_MAP(CCheckerBoardImageDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCheckerBoardImageDlg::OnBnClickedGetColors)
	ON_BN_CLICKED(IDC_CREATE_IMAGE, &CCheckerBoardImageDlg::OnBnClickedCreateImage)
	ON_BN_CLICKED(IDC_RANDOM, &CCheckerBoardImageDlg::OnBnClickedRandom)
	ON_BN_CLICKED(IDC_RADIO_1, &CCheckerBoardImageDlg::OnBnClickedMode)
	ON_BN_CLICKED(IDC_RADIO_OR2, &CCheckerBoardImageDlg::OnBnClickedMode)
	ON_BN_CLICKED(IDC_RADIO_XOR2, &CCheckerBoardImageDlg::OnBnClickedMode)
	ON_BN_CLICKED(IDC_LOAD_CONFIG, &CCheckerBoardImageDlg::OnBnClickedLoadConfig)
	ON_EN_CHANGE(IDC_WIDTH, &CCheckerBoardImageDlg::OnEnChangeWidth)
	ON_EN_CHANGE(IDC_HEIGHT, &CCheckerBoardImageDlg::OnEnChangeHeight)
	ON_EN_KILLFOCUS(IDC_WIDTH, &CCheckerBoardImageDlg::OnEnKillfocusWidth)
	ON_BN_CLICKED(IDC_TEST, &CCheckerBoardImageDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_NEXTIMAGE, &CCheckerBoardImageDlg::OnBnClickedNextimage)
	ON_BN_CLICKED(IDC_RANDOM_PAIRS, &CCheckerBoardImageDlg::OnBnClickedRandomPairs)
	ON_BN_CLICKED(IDC_RESET, &CCheckerBoardImageDlg::OnBnClickedLayoutReset)
	ON_BN_CLICKED(IDC_RESET_PAIRS, &CCheckerBoardImageDlg::OnBnClickedResetPairs)
	ON_BN_CLICKED(IDC_CHECK_RANDOM, &CCheckerBoardImageDlg::OnBnClickedCheckRandom)
	//ON_BN_CLICKED(IDC_RADIO_SQUARE_WITH_HOLE2, &CCheckerBoardImageDlg::OnBnClickedRadioSquareWithHole2)
	//ON_BN_CLICKED(IDC_RADIO_QUADRILATERAL, &CCheckerBoardImageDlg::OnBnClickedRadioQuadrilateral)
	//ON_BN_CLICKED(IDC_RADIO_BROKEN_SQUARE2, &CCheckerBoardImageDlg::OnBnClickedRadioBrokenSquare2)
END_MESSAGE_MAP()


// CCheckerBoardImageDlg message handlers

BOOL CCheckerBoardImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	for (auto i = 0; i < 8; i++)
	{
		layerPairValues[i][0] = (i + 1) % 8;
		//layerPairValues[i][0] = i;
		layerPairValues[i][1] = (i+1) % 8;
	}

	m_width = 720;
	m_height =  900;
	m_numberOfImages = 1;
	m_random = FALSE;
	m_layerMode = SINGLE_LAYER;
	UpdateData(FALSE);	// to write out height and width

	ShowLayerPairs(FALSE);
	CreateRowColDropLists();
	SetRandomSeed();
	OnBnClickedRandom();
	m_refreshRandomCtrl.EnableWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCheckerBoardImageDlg::OnPaint()
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
HCURSOR CCheckerBoardImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCheckerBoardImageDlg::OnBnClickedGetColors()
{
	////////////////////////////////////////////////////
	// Get color table file name
	//CFileDialog dlgFile(TRUE, CString(".txt"), NULL, 0, CString("Text Files (*.txt)|*.txt|"));
	CFileDialog dlgFile(TRUE, CString(".bmp"), NULL, 0, CString("BMP Files (*.bmp)|*.bmp|"));
	OPENFILENAME& ifn = dlgFile.GetOFN();
	if (!colorTableFolder.IsEmpty())
	{
		// Set dialog to open i last folder for color tables
		//OPENFILENAME& ifn = dlgFile.GetOFN();
		ifn.lpstrInitialDir = colorTableFolder;
	}
	else
	{
		//L"K:\\Users\\Steve\\CSUSM-ArtProject\\ColorTables";
		CString colorTableFolder;
		colorTableFolder.LoadStringW(IDS_COLOR_TABLE_FOLDER);
		dlgFile.m_ofn.lpstrInitialDir = L"K:\\Users\\Steve\\CSUSM-ArtProject\\ColorTables";;
			//colorTableFolder.GetBuffer(_MAX_PATH);;
		//ifn.lpstrInitialDir = colorTableFolder;
	}

	CString bmpFileName;

	if (dlgFile.DoModal() == IDOK)
	{
		bmpFileName = dlgFile.GetPathName();
		colorTableFolder = dlgFile.GetFolderPath();
	}
	else
	{
		AfxMessageBox(_T("Cannot Read Color Table"));
		return;
	}

	// Replace bmp with txt in file name
	CString txtFileName = BmpToTxt(bmpFileName);

	ReadColorTable(txtFileName);
}

void CCheckerBoardImageDlg::ReadColorTable(CString txtFileName)
{
	////////////////////////////////////////////////////
	// Read Color Table
	TCHAR buffer[200];
	FILE *colors;
	colors = _tfopen(txtFileName, _T("r"));

	int safety;
	_fgetts(buffer, 199, colors);
	_stscanf(buffer, _T("%d"), &safety);
	if (safety != COLORS_SAFETY)
	{
		MessageBox((LPCTSTR) "Incorrect File Type");
		return;
	}

	// There should be 256 lines of color info
	for (auto i = 0; i < 256; i++)
	{
		int ix, r, g, b;
		GetNextLine(colors, buffer, 199);
		_stscanf(buffer, _T("%d %d %d %d)"), &ix, &r, &g, &b);
		red[ix] = r;
		green[ix] = g;
		blue[ix] = b;
	}
 	colorsRead = true;

	////////////////////////////////////////////////////////////////////////////
	//// Create RGBQUAD array for colortable
	pRgbQuad = &pallette[0];

	for (auto i = 0; i < 256; i++) {
		pRgbQuad->rgbRed = (BYTE)red[i];
		pRgbQuad->rgbGreen = (BYTE)green[i];
		pRgbQuad->rgbBlue = (BYTE)blue[i];
		pRgbQuad->rgbReserved = (BYTE)0;

		//char buffer[100];
		//sprintf_s(buffer, "I %3d:  R %3d  G %3d  B %3d  addr %x\n", i, red[i], green[i], blue[i], pRgbQuad);
		//OutputDebugStringA(buffer);

		pRgbQuad++;
	}
}

//  The line read is put into buffer, a global variable in this class
//	Not the best way of doing this since it is a hidden side effect.
//	Tried several other explicit ways of doing this, 
//	but kept having character set conversion problems
//void CCheckerBoardImageDlg::GetNextLine(CStdioFile *file)
void CCheckerBoardImageDlg::GetNextLine(FILE* fp, LPTSTR buf, int buflen)
{
	// skip comment lines starting with #
	do
	{
		_fgetts(buf, 199, fp);
	} while
		(buf[0] == _T('#'));
}

void CCheckerBoardImageDlg::TraceBMP(BITMAPFILEHEADER bmfh, BITMAPINFOHEADER bmih) //, RGBQUAD* colortable, UCHAR* pixelData)
{
	TT("bmfh.bfType", bmfh.bfType);  O(bfr);
	TT("bmfh.bfSize", bmfh.bfSize);  O(bfr);
	TT("bmfh.bfReserved1", bmfh.bfReserved1);  O(bfr);
	TT("bmfh.bfReserved2", bmfh.bfReserved2);  O(bfr);
	TT("bmfh.bfOffBits", bmfh.bfOffBits);  O(bfr);
	TT("bmih.biSize", bmih.biSize);  O(bfr);
	TT("bmih.biWidth", bmih.biWidth);  O(bfr);
	TT("bmih.biHeight", bmih.biHeight);  O(bfr);
	TT("bmih.biPlanes", bmih.biPlanes);  O(bfr);
	TT("bmih.biBitCount", bmih.biBitCount);  O(bfr);
	TT("bmih.biCompression", bmih.biCompression);  O(bfr);
	TT("bmih.biSizeImage", bmih.biSizeImage);  O(bfr);
	TT("bmih.biXPelsPerMeter", bmih.biXPelsPerMeter);  O(bfr);
	TT("bmih.biYPelsPerMeter", bmih.biYPelsPerMeter);  O(bfr);
	TT("bmih.biClrUsed", bmih.biClrUsed);  O(bfr);
	TT("bmih.biClrImportant", bmih.biClrImportant);  O(bfr);
	sprintf_s(bfr, "pRgbQuad    %x  %d\n", pRgbQuad, pRgbQuad); O(bfr);
	sprintf_s(bfr, "pRgbQuad[0]    %x  %d\n", pRgbQuad[0], pRgbQuad[0]); O(bfr);
}

void CCheckerBoardImageDlg::SaveBMP(LPCTSTR Filename, clock_t start) // , UCHAR* colortable, UCHAR* pixelData)
{
	BITMAPFILEHEADER    bmfh;  //stores information about the file format
	BITMAPINFOHEADER    bmih;  //stores information about the bitmap
	CFile               file;  //stores file pointer

	//create bitmap file header
	((unsigned char *)&bmfh.bfType)[0] = 'B';
	((unsigned char *)&bmfh.bfType)[1] = 'M';
	bmfh.bfSize = 54 + m_height * m_width + 1024;	// 1024 for color table
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = 54;	// sizeof(BITMAPFILEHEADER) + 
							// sizeof(BITMAPINFOHEADER);

	//create bitmap information header
	bmih.biSize = 40;		//  sizeof(BITMAPINFOHEADER)
	bmih.biWidth = m_width;
	bmih.biHeight = m_height;
	bmih.biPlanes = 1;		// always 1
	bmih.biBitCount = 8;	// Using Colortable
	bmih.biCompression = 0;	// BI_RGB = 0
	bmih.biSizeImage = 0;	// Not needed for BI_RGB
	bmih.biXPelsPerMeter = 3800;	// can be anything
	bmih.biYPelsPerMeter = 3800;	// can be anything
	bmih.biClrUsed = 256;		// Use all colors in the Color Table
	bmih.biClrImportant = 0;	// 0 = use all colors

	//TraceBMP(bmfh, bmih); // , pRgbQuad, pixelIndex);

	//save all header and bitmap information into file
	if (!file.Open(Filename, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox(_T("Failed to create file"));
	} 
	else
	{
		file.Write(&bmfh, sizeof(BITMAPFILEHEADER));
		file.Write(&bmih, sizeof(BITMAPINFOHEADER));
		file.Write((void*)pallette, 1024);	// 1024 = 256 entries * 4 bytes per entry
		int rowSize = ceil((bmih.biBitCount * m_width) / 32) * 4;
		file.Write(pixelIndex, rowSize * m_height);
		file.Close();

		if (start != -1)
		{
			clock_t ticks = clock() - start;
			char buffer[100];
			sprintf_s(buffer, "**\n**\nTime was %d clock ticks   %f sec\n**\n", ticks, (double)ticks / CLOCKS_PER_SEC);
			OutputDebugStringA(buffer);
			sprintf_s(buffer, "CLOCKS_PER_SEC %d\n**\n", CLOCKS_PER_SEC);
			OutputDebugStringA(buffer);
		}
	}
}

// Create Basic CheckerBoards
void CCheckerBoardImageDlg::CreateBasicCheckerBoards()
{
	UpdateData(TRUE);

	for (auto i = 0; i < 8; i++)
	{
		switch (m_shape)
		{
		case TRIANGLE:	chBase[i] = new TriangleBoard(m_width, m_height, m_cols[i], m_rows[i]);
			break;
		case RECTANGLE:
			chBase[i] = new CheckerBoard(m_width, m_height, m_cols[i], m_rows[i], m_random);
			break;
		case HOLE_IN_SQR:  chBase[i] = new SquareWithHole(m_width, m_height, m_cols[i], m_rows[i]);
			break;
		case ELLIPSE:	chBase[i] = new EllipseBoard(m_width, m_height, m_cols[i], m_rows[i], m_random);
			break;
		case BROKEN_SQR:	chBase[i] = new BrokenSqr(m_width, m_height, m_cols[i], m_rows[i], m_random);
			break;
		case QUADRILATERAL:	chBase[i] = new Quadrilateral(m_width, m_height, m_cols[i], m_rows[i], m_random);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}
}

// Create Pixel Index array
void CCheckerBoardImageDlg::CreatePixelIndex()
{
	pixelIndex = new UINT8[m_width*m_height];

	///////////////////////////////////////////////////////////
	//// Calculate Pixel Index values and place in array
	UINT8 bitValues[8] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80 };
	for (auto row = 0; row < m_height; row++)
		for (auto col = 0; col < m_width; col++)
		{
			UINT8 pixelVal = 0;
			for (auto lyr = 0; lyr < 8; lyr++)
			{
				if (ch[lyr]->GetAt(row, col))
				{
					pixelVal += bitValues[7 - lyr];
				}
			}
			pixelIndex[row*m_width + col] = pixelVal;
		}
}

void CCheckerBoardImageDlg::CreateTwoLayerPixelIndex(boolean xor)
{
	CreateBasicCheckerBoards();

	//////////////////////////////////////////////////////////////
	//// Create OR'd or XOR'ed CheckerBoards
	for (auto i = 0; i < 8; i++)
	{
		//BooleanBitmap* left = chBase[layerPairValues[i][0]];
		//BooleanBitmap* right = chBase[layerPairValues[i][1]];
		BooleanBitmap* left = chBase[i];
		BooleanBitmap* right = chBase[layerPairValues[i][0]];
		ch[i] = new BooleanBitmap(left, right, xor );
	}
	CreatePixelIndex();
}

void CCheckerBoardImageDlg::CreateOneLayerPixelIndex()
{
	CreateBasicCheckerBoards();

	// Final boards are just the base boards
	for (auto i = 0; i < 8; i++)
		ch[i] = chBase[i];

	//////////////////////////////////////////////////////////////
	// Create Pixel Index array
	CreatePixelIndex();
}

boolean CCheckerBoardImageDlg::VerifyInput()
{
	UpdateData(TRUE);
	////////////////////////////////////////////////////////////////
	// Verify everything is defined before proceeding
	if (
		m_rows[0] == 0 || m_rows[1] == 0 || m_rows[2] == 0 || m_rows[3] == 0 ||
		m_rows[4] == 0 || m_rows[5] == 0 || m_rows[6] == 0 || m_rows[7] == 0 ||
		m_cols[0] == 0 || m_cols[1] == 0 || m_cols[2] == 0 || m_cols[3] == 0 ||
		m_cols[4] == 0 || m_cols[5] == 0 || m_cols[6] == 0 || m_cols[7] == 0 ||
		m_width == 0 || m_height == 0
		)
	{
		AfxMessageBox(_T("Layers Not Defined"));
		return false;
	}
	if (!colorsRead)
	{
		AfxMessageBox(_T("No Color Info"));
		return false;
	}
	return true;
}

void CCheckerBoardImageDlg::OnBnClickedRandom()
{
	RandomRowsCols();
}

void CCheckerBoardImageDlg::RandomLayerPairs()
{
	//////////////////////////////////////////////////////////////
	//// Create OR'd or XOR'ed CheckerBoards
	for (auto i = 0; i < 8; i++)
	{
		//// Chose two different Base Checkerboards
		//int chb1 = rand() % 8;
		//int chb2 = rand() % 8;
		//while (chb1 == chb2)
		//	chb2 = rand() % 8;

		//layerPairValues[i][0] = chb1;
		//layerPairValues[i][1] = chb2;

		// Chose Base Different from one to be paired with
		int pairWith;
		do { pairWith = rand() % 8; } 
		while (pairWith == i);

		layerPairValues[i][0] = pairWith;
		layerPairValues[i][1] = pairWith;
		
	}
	UpdateData(FALSE);
}

void CCheckerBoardImageDlg::RandomRowsCols()
{
	CreateRowColDropLists();

	// The idea her is to restrict the random values

	const int LOW_LIMIT = 5; 
	const int MEDIUM_LIMIT = 10;
	const int LARGE_LIMIT = 80;
	// Above LARGE are considered HUGE

	int nWidth = widthDivisors.size();
	int nHeight = heightDivisors.size();

	//int n = (int)(nHeight*0.4);
	//m_rows[0] = heightDivisors[rand() % (n == 0 ? 1 : n)];
	//n = (int)(nHeight*0.6);
	//m_rows[1] = heightDivisors[rand() %( n == 0 ? 1 : n)];
	//m_rows[2] = heightDivisors[rand() % nHeight];
	//m_rows[3] = heightDivisors[rand() % nHeight];
	//m_rows[4] = heightDivisors[rand() % nHeight];
	//m_rows[5] = heightDivisors[rand() % nHeight];
	//m_rows[6] = heightDivisors[rand() % nHeight];
	//m_rows[7] = heightDivisors[rand() % nHeight];

	//n = (int)(nWidth*0.4);
	//m_cols[0] = widthDivisors[rand() % (n == 0 ? 1 : n)];
	//n = (int)(nWidth*0.6);
	//m_cols[1] = widthDivisors[rand() % (n == 0 ? 1 : n)];
	//m_cols[2] = widthDivisors[rand() % nWidth];
	//m_cols[3] = widthDivisors[rand() % nWidth];
	//m_cols[4] = widthDivisors[rand() % nWidth];
	//m_cols[5] = widthDivisors[rand() % nWidth];
	//m_cols[6] = widthDivisors[rand() % nWidth];
	//m_cols[7] = widthDivisors[rand() % nWidth];

	int nWidthSmall = 0;
	int nWidthMedium = 0;
	int nWidthLarge = 0;
	for (auto i = 0; i < nWidth; i++)
	{
		int w = widthDivisors[i];
		if (w <= LOW_LIMIT) nWidthSmall++;
		if (w <= MEDIUM_LIMIT) nWidthMedium++;
		if (w <= LARGE_LIMIT) nWidthLarge++;
	}
	int nHeightSmall = 0;
	int nHeightMedium = 0; 
	int nHeightLarge = 0;
	for (auto i = 0; i < nHeight; i++)
	{
		int h = heightDivisors[i];
		if (h <= LOW_LIMIT) nHeightSmall++;
		if (h <= MEDIUM_LIMIT) nHeightMedium++;
		if (h <= LARGE_LIMIT) nHeightLarge++;
	}

	// This value is calculated because it can cause an assertion in some certances
	int low = (nHeight - nHeightLarge <=  2 ) ? 0 : nHeightLarge;
	m_rows[0] = heightDivisors[TriDist(0, nHeightSmall)];
	m_rows[1] = heightDivisors[TriDist(0, nHeightSmall)];
	m_rows[2] = heightDivisors[TriDist(0, nHeightMedium)];
	m_rows[3] = heightDivisors[TriDist(0, nHeightMedium)];
	m_rows[4] = heightDivisors[TriDist(0, nHeightLarge)];
	m_rows[5] = heightDivisors[TriDist(low, nHeight)];
	m_rows[6] = heightDivisors[TriDist(0, nHeightLarge)];
	m_rows[7] = heightDivisors[TriDist(low, nHeight)];

	low = (nWidth - nWidthLarge <= 2) ? 0 : nHeightLarge;
	m_cols[0] = widthDivisors[TriDist(0, nWidthSmall)];
	m_cols[1] = widthDivisors[TriDist(0, nWidthSmall)];
	m_cols[2] = widthDivisors[TriDist(0, nWidthMedium)];
	m_cols[3] = widthDivisors[TriDist(0, nWidthMedium)];
	m_cols[4] = widthDivisors[TriDist(0, nWidthLarge)];
	m_cols[5] = widthDivisors[TriDist(low, nWidth)];
	m_cols[6] = widthDivisors[TriDist(0, nWidthLarge)];
	m_cols[7] = widthDivisors[TriDist(low, nWidth)];

	SetRowColDropLists();
	UpdateData(FALSE);
}

int CCheckerBoardImageDlg::TriDist(int a, int b)
{
	double halfB = ((double)b) / 2.0;
	double mode = (halfB < (double)a) ? ((a + b)/ 2.0) : halfB;
	ASSERT(a <= mode && mode <= b);
	return (int)TriangleDistribution((double)a, (double)b, mode);
}

double CCheckerBoardImageDlg::TriangleDistribution(double min, double max, double mode)
{
	// min is beginning of interval
	// max is end of interval
	// mode is x value of the mode 
	// https://en.wikipedia.org/wiki/Triangular_distribution
	ASSERT(min <= mode && mode <= max);
	double modePt = (mode - min) / (max - min);
	double u = ((double)rand()) / (double)RAND_MAX;
	int n = RAND_MAX;
	return (u < modePt) ?
		min + sqrt(u * (max - min) * (mode - min)) :
		max - sqrt((1.0 - u) * (max - min) *(max - mode));
}

void CCheckerBoardImageDlg::CreateRowColDropLists()
{
	UpdateData(TRUE);

	GetDivisors(m_height, &heightDivisors);
	GetDivisors(m_width, &widthDivisors);

	// Clear out (possible) previous values in drop down lists

	for (auto i = 0; i < 8; i++)
	{
		char str[200];
		m_comboRow[i].ResetContent();
		//sprintf_s(bfr, "Row Drop Down %d has %d items\n", i, m_comboRow[i].GetCount());//O();
		//OutputDebugStringA(bfr);

		m_comboCol[i].ResetContent();
		//sprintf_s(bfr, "Col Drop Down %d has %d items\n", i, m_comboCol[i].GetCount());//O();
		//OutputDebugStringA(bfr);
	}

	///////////////////////////////////////////////////////////////
	// Initialize Drop Down Lists
	vector<int>::iterator it;
	TCHAR s[10];
	LPCTSTR  str;
	int dropListIx = 0;
	for (it = heightDivisors.begin(); it != heightDivisors.end(); it++)
	{
		str = _itot(*it, s, 10);
		for (auto i = 0; i < 8; i++)
		{
			m_comboRow[i].AddString(str);
			m_comboRow[i].SetItemData(dropListIx, *it);
		}
		dropListIx++;
	}
	dropListIx = 0;
	for (it = widthDivisors.begin(); it != widthDivisors.end(); it++)
	{
		str = _itot(*it, s, 10);
		for (auto i = 0; i < 8; i++)
		{
			m_comboCol[i].AddString(str);
			m_comboCol[i].SetItemData(dropListIx, *it);
		}
		dropListIx++;
	}

	//for (auto i = 0; i < 8; i++)
	//{
	//	char str[200];
	//	sprintf_s(bfr, "Row Drop Down %d has %d items\n", i, m_comboRow[i].GetCount());//O();
	//	OutputDebugStringA(bfr);
	//	sprintf_s(bfr, "Col Drop Down %d has %d items\n", i, m_comboCol[i].GetCount());//O();
	//	OutputDebugStringA(bfr);
	//}
}

CString CCheckerBoardImageDlg::GetOutputFolderName()
{
	CFolderPickerDialog dlgFolder;
	CString folderName;
	if (dlgFolder.DoModal() == IDOK)
	{
		folderName = dlgFolder.GetPathName();
	}
	else
	{
		AfxMessageBox(_T("Cannot Save CheckerBoard Images"));
	}
	return folderName;
}

CString CCheckerBoardImageDlg::GetOutputFileName()
{
	CString fileName;
	boolean createdNewFilename = false;

	if (nextImageClicked && !lastFileName.IsEmpty())
	{
		// assumes last file name ends in number
		// separate number from file name prefix
		// add 1 and generate new file name.
		int len = lastFileName.GetLength();
		// Check last 4 are .bmp and remove
		CString bmp = lastFileName.Right(4);
		if (bmp.CollateNoCase(_T(".bmp")) == 0)
		{
			// find beginning of number at end of string -- scan backwards
			
			CString fileNameRoot = lastFileName.Left(len - 4);
			int i = len - 5; // 4 for removing .bmp and 1 more since last char is at len-1
			while (isdigit(fileNameRoot[i]))
			{
				i--;
			}

			// file prefix ends at i
			int rootlen = len - 4;
			CString fileprefix = fileNameRoot.Left(i+1);
			CString numberStr = fileNameRoot.Right(rootlen - i-1);
			int n = _ttoi(numberStr.GetBuffer());
			CString newNumber = _T("     ");
			newNumber.Format(_T("%d.bmp"), ++n);
			fileName = fileprefix + newNumber;
			createdNewFilename = true;
		}
	}
	if ( !createdNewFilename)
	{
		CFileDialog dlgFile(FALSE, CString(".bmp"), NULL, OFN_OVERWRITEPROMPT, CString("BMP Files (*.bmp)|*.bmp|"));
		OPENFILENAME& ifn = dlgFile.GetOFN();
		if (!imageFolder.IsEmpty())
		{
			// Set dialog to open i last folder for color tables
			//OPENFILENAME& ifn = dlgFile.GetOFN();
			ifn.lpstrInitialDir = imageFolder;
		}
		else
		{
			CString folderName;
			folderName.LoadString(IDS_IMAGES_FOLDER);
			//ifn.lpstrInitialDir = folderName;
			ifn.lpstrInitialDir = L"K:\\Users\\Steve\\CSUSM-ArtProject\\Images";
			//dlgFile.m_ofn.lpstrInitialDir = folderName;
		}


		if (dlgFile.DoModal() == IDOK)
		{
			fileName = dlgFile.GetPathName();
			imageFolder = dlgFile.GetFolderPath();
		}
		else
		{
			AfxMessageBox(_T("Cannot Save Image"));
		}
	}
	lastFileName = fileName;
	return fileName;
}

void CCheckerBoardImageDlg::TwoLayeredImages(boolean xor)
{
	ReadRowColDropLists();
	if (!VerifyInput())
		return;

	//// Get output file name
	CString fileName = GetOutputFileName();

	clock_t start = clock();
	////////////////////////////////////////////////////////////////////////////
	//// Create values of the pixels from the checkopard group
	CreateTwoLayerPixelIndex(xor);

	// Create BMP file and write it out
	SaveBMP(fileName, start);
	SaveConfiguration(fileName);
	CleanUp();
}

void CCheckerBoardImageDlg::SetRandomSeed()
{
	// If a config file was just loaded (configFileLoaded==true) OR
	// If the "Refresh Random" is not checked, do NOT genetrate a new random seeed
	// use the existing one in m_startTime
	if (configFileLoaded || !m_refreshRandom)
	{
		// After a LoadConfiguration, this is set to false, 
		// so we use seed saved in config file.
		// Next time back to generating a new seed for each image created
		configFileLoaded = false;
	}
	else
	{
		struct _timeb timeptr;
		_ftime_s(&timeptr);
		m_startTime = (timeptr.time * 1000 + timeptr.millitm) & 0xffffffff;
	}
	srand((UINT)m_startTime);
}


void CCheckerBoardImageDlg::OnBnClickedCreateImage()
{
	UpdateData(TRUE);
	ReadRowColDropLists();

	SetRandomSeed();

	switch (m_layerMode)
	{
	case SINGLE_LAYER:
		SingleLayer();
		break;
	case TWO_OR:
		TwoLayeredImages(false);
		break;
	case TWO_XOR:
		TwoLayeredImages(true);
		break;
	default:
		AfxMessageBox(_T("Layer Moder Error"));
		return;
	}
}

void CCheckerBoardImageDlg::GetTimeString(LPTSTR buf, int buflen)
{
	time_t rawtime;
	struct tm * timeinfo;
	//char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	_tcsftime(buf, 80, _T("%Y%m%d%H%M%S"), timeinfo);
}

void CCheckerBoardImageDlg::SingleLayerMultiple()
{
	// Get Output folder
	CString folderName = GetOutputFolderName();

	// Base for File Name
	TCHAR timestr[80];
	GetTimeString(timestr, 80);
	CString fileNameBase = folderName + _T("\\") + _T("ChkBrd-") + timestr;// +_T("-");

	for (auto i = 1; i <= m_numberOfImages; i++)
	{
		// Randomize
		SetRandomSeed();
		OnBnClickedRandom();

		//// Create values of the pixels from the checkopard group
		CreateOneLayerPixelIndex();

		// Create BMP file and write it out
		CString fileN;
		fileN.Format(_T("-%d.bmp"), i);
		CString fileName = fileNameBase + fileN;// +_T(".bmp";)

		SaveBMP(fileName);
		SaveConfiguration(fileName);
		CleanUp();
	}

}

void CCheckerBoardImageDlg::SingleLayer()
{
	if (!VerifyInput())
		return;

	if (m_numberOfImages > 1)
	{
		SingleLayerMultiple();
		return;
	}

	//// Get output file name
	CString fileName = GetOutputFileName();
	time_t startTime = clock();

	//// Create values of the pixels from the checkopard group
	CreateOneLayerPixelIndex();

	// Create BMP file and write it out

	SaveBMP(fileName, startTime); // , m_height, m_width); // , (UINT8*)&pallette[0], (UINT8*)&pixelIndex[0]);
	SaveConfiguration(fileName);
	CleanUp();
}

// After I removed the Draw() function from the subclasses of CheckerBoard
// and made Fill(..) a virtual function in CheckerBoard
// I haven't determined why this happens.
// Ignore them for now even though it will cause memory leaks.
void CCheckerBoardImageDlg::CleanUp()
{
	// delete checker boards and pixlvalues
	for (auto i = 0; i < 8; i++)
	{
		try { delete ch[i]; }
		catch (...)	{ };
		if (m_layerMode != SINGLE_LAYER)
		{
			try { delete chBase[i]; }
			catch (...) {};
		}
	}
	delete pixelIndex;
}


CString CCheckerBoardImageDlg::TxtToBmp(CString fromTXT)
{
	///////////////////////////////////////////////
	// Fix File Name if needed
	// remove .bmp and add .txt
	CString txt = _T(".txt");
	CString bmp = _T(".bmp");
	CString toTxt;
	int loc = fromTXT.Find(txt);
	if (loc > 0)
	{
		toTxt = fromTXT.Left(fromTXT.GetLength() - 4) + bmp;
	}
	else
	{
		toTxt = fromTXT + bmp;
	}
	return toTxt;
}

char* CCheckerBoardImageDlg::CStringToCharStar(CString cs)
{
	// Convert to a char* string from a wide character
// CStringW string. To be safe, we allocate two bytes for each
// character in the original string, including the terminating
// null.
	const size_t newsizew = (cs.GetLength() + 1) * 2;
	char* charString = new char[newsizew];
	size_t convertedCharsw = 0;
	wcstombs_s(&convertedCharsw, charString, newsizew, cs, _TRUNCATE);
	return charString;
}

CString CCheckerBoardImageDlg::BmpToTxt(CString fromBMP)
{
	///////////////////////////////////////////////
	// Fix File Name if needed
	// remove .bmp and add .txt
	CString txt = _T(".txt");
	CString toTxt;
	int loc = fromBMP.Find(_T(".bmp"));
	if (loc > 0)
	{
		toTxt = fromBMP.Left(fromBMP.GetLength() - 4) + txt;
	}
	else
	{
		toTxt = fromBMP + txt;
	}
	return toTxt;
}

//  This corresponds to current config defn
// Earlier version is no longer written out
// but it can be read back with limited info
//
boolean CCheckerBoardImageDlg::SaveConfiguration(CString fileName)
{
#define O()  file.Write((LPCTSTR)str, strlen(str))
	char str[200];

	CString szPathName = BmpToTxt(fileName);

	//////////////////////////////////////////////////////////////
	// Open File
	CStdioFile file;
	UINT flags = CFile::modeCreate | CFile::modeWrite;// | CFile::typeText;
	CFileException e;
	try
	{
		BOOL res = file.Open(szPathName, flags, &e);
		sprintf_s(bfr, "File created:  %s  Ret Val %d\n", szPathName, res);//O();
		OutputDebugStringA(bfr);
	}
	catch (...)
	{
		CString err;
		err.GetBuffer(100);
		e.GetErrorMessage(err.GetBuffer(), 100);
		AfxMessageBox(err);
		return false;
	}

	//////////////////////////////////////////////////////////////
	//// Ready to write

	// Safety Value - check this when we read it to avoid reading an inappropriate file
	sprintf_s(str, "#  %s\n\0", "CHECKER_BOARD_SAFETY_VALUE_2"); O();
	sprintf_s(str, "%d\n\0", CHECKER_BOARD_SAFETY_VALUE_2); O();

	sprintf_s(str, "#\n#  %s\n#\n\0", "Pixel Width and Height"); O();
	sprintf_s(str, "%d %d\n\0", m_width, m_height); O();

	// Random flag
	sprintf_s(str, "#\n#  %s\n#\n\0", "Random Flag - is it used"); O();
	sprintf_s(str, "%u\n\0", m_random); O();

	// seed
	sprintf_s(str, "#\n#  %s\n#\n\0", "Start Time possibly used for Random Seed"); O();
	sprintf_s(str, "%u\n\0", m_startTime); O();

	// File Type:enum Single layer, Or2Layers, Xor2Layers
	sprintf_s(str, "#\n#  %s\n#\n\0", "File Type: Single layer(0), Or2Layers(1), Xor2Layers(2)"); O();
	sprintf_s(str, "%d\n\0", m_layerMode); O();

	// Object shape  --enum SHAPE { RECTANGLE, TRIANGLE, ELLIPSE };
	sprintf_s(str, "#\n#  %s\n#\n\0", "Shape Type: RECTANGLE(0), TRIANGLE(1), ELLIPSE(2)"); O();
	sprintf_s(str, "%d\n\0", m_shape); O();

	// Additional Numerical Parameter Pairs - future use
	sprintf_s(str, "#\n#  %s\n#\n\0", "Parameter Pairs"); O();
	sprintf_s(str, "%d\n\0", 0); O();

	// Additional String Parameter Pairs
	sprintf_s(str, "#\n#  %s\n#\n\0", "Parameter Pairs"); O();
	sprintf_s(str, "%d\n\0", 0); O();

	//sprintf_s(str, "#\n#  %s\n#\n\0", "Color Table Name"); O();
	//char* colorTable = CStringToCharStar(colorTableFolder);
	//sprintf_s(str, "%d %s\n\0", COLORTABLE, colorTable); O();

	// Write rows and cols
	sprintf_s(str, "#\n#  %s\n#\n\0", "8 Row col values for each layer"); O();
	for (auto i = 0; i < 8; i++)
	{
		sprintf_s(str, "%d %d\n\0", m_rows[i], m_cols[i]); O();
	}

	// Layer pairs (if relevant)
	if (m_layerMode == TWO_OR || m_layerMode == TWO_XOR)
	{
		sprintf_s(str, "#\n#  %s\n#\n\0", "8 layer pairs for OR or XOR"); O();
		for (auto i = 0; i < 8; i++)
		{
			sprintf_s(str, "%d %d\n\0", layerPairValues[i][0], layerPairValues[i][1]); O();
		}
	}
	file.Close();
	return true;
}

boolean CCheckerBoardImageDlg::LoadConfiguration()
{
#define O()  file.Write((LPCTSTR)str, strlen(str))
	char str[200];

	//CFileDialog dlgFile(TRUE, CString(".txt"), NULL, OFN_OVERWRITEPROMPT, CString("TXT Files (*.txt)|*.txt|"));
	CFileDialog dlgFile(TRUE, CString(".bmp"), NULL, OFN_OVERWRITEPROMPT, CString("BMP Files (*.bmp)|*.bmp|"));
	
	CString folderName;
	folderName.LoadString(IDS_IMAGES_FOLDER);
	dlgFile.m_ofn.lpstrInitialDir = folderName;
	
	CString bmpFileName;
	if (dlgFile.DoModal() == IDOK)
	{
		bmpFileName = dlgFile.GetPathName();
	}
	else
	{
		AfxMessageBox(_T("Cannot Load Image Config File"));
		return false;
	}

	// Replace bmp with txt in file name
	CString txtFileName = BmpToTxt(bmpFileName);

	//////////////////////////////////////////////////////////////
	// Open File
	TCHAR buffer[200];
	FILE *config;
	config = _tfopen(txtFileName, _T("r"));

	if ( config == NULL )
	{
		MessageBox((LPCTSTR) "Config File Does Not Exist");
		return false;
	}

	int safety;
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d"), &safety);
	if (safety == CHECKER_BOARD_SAFETY_VALUE) // Legacy
	{
		LoadConfiguration1(config);
	}
	else if(safety == CHECKER_BOARD_SAFETY_VALUE_2)
	{
		LoadConfiguration2(config);
	}
	else
	{
		MessageBox((LPCTSTR) "Incorrect File Type");
		return false;
	}

	//// Width and Height
	//GetNextLine(config, buffer, 199);
	//_stscanf(buffer, _T("%d %d"), &m_width, &m_height);

	//// Start Time (random seed)
	//GetNextLine(config, buffer, 199);
	//_stscanf(buffer, _T("%d"), &m_startTime);

	//// File Type - m_layerMode
	//GetNextLine(config, buffer, 199);
	//_stscanf(buffer, _T("%d"), &m_layerMode);

	//// There should be 8 pairs of row and col info
	//for (auto i = 0; i < 8; i++)
	//{
	//	GetNextLine(config, buffer, 199);
	//	_stscanf(buffer, _T("%d %d"), &m_rows[i], &m_cols[i]);
	//}

	//if (m_layerMode == TWO_OR || m_layerMode == TWO_XOR)
	//{
	//	for (auto i = 0; i < 8; i++)
	//	{
	//		GetNextLine(config, buffer, 199);
	//		_stscanf(buffer, _T("%d %d"), &layerPairValues[i][0], &layerPairValues[i][1]);
	//	}
	//}
	fclose(config);

	SetRowColDropLists();
	ShowLayerPairs(m_layerMode != SINGLE_LAYER);

	configFileLoaded = true;
	srand((UINT)m_startTime);	// reset random seed

	UpdateData(FALSE);
	return true;
}

// Version 1 of Config file Format
boolean CCheckerBoardImageDlg::LoadConfiguration1(FILE *config)
{
	TCHAR buffer[200];

	// Width and Height
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d %d"), &m_width, &m_height);

	// Start Time (random seed)
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d"), &m_startTime);

	// File Type - m_layerMode
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d"), &m_layerMode);


	// There should be 8 pairs of row and col info
	for (auto i = 0; i < 8; i++)
	{
		GetNextLine(config, buffer, 199);
		_stscanf(buffer, _T("%d %d"), &m_rows[i], &m_cols[i]);
	}

	if (m_layerMode == TWO_OR || m_layerMode == TWO_XOR)
	{
		for (auto i = 0; i < 8; i++)
		{
			GetNextLine(config, buffer, 199);
			_stscanf(buffer, _T("%d %d"), &layerPairValues[i][0], &layerPairValues[i][1]);
		}
	}
	return true;
}

boolean CCheckerBoardImageDlg::LoadConfiguration2(FILE *config)
{
	TCHAR buffer[200];

	// Width and Height
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d %d"), &m_width, &m_height);

	// Random flag
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d"), &m_random);

	// Start Time (random seed)
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d"), &m_startTime);

	// File Type - m_layerMode
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d"), &m_layerMode);

	// Object Shape - m_shape
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d"), &m_shape);

	// Number of additional numerical parameters 
	// in pairs - 
	//	number to indicate which parameter it is
	//	number to indicate parameter value
	int nParamPairs;
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d"), &nParamPairs);
	for (auto i = 0; i < nParamPairs; i++)
	{
		int param, value;
		GetNextLine(config, buffer, 199);
		_stscanf(buffer, _T("%d %d"), &param, &value);
		SaveConfigParam(param, value);
	}

	// Number of additional text parameters 
	// in pairs - 
	//	number to indicate which parameter it is
	//	number to indicate parameter value
	nParamPairs;
	GetNextLine(config, buffer, 199);
	_stscanf(buffer, _T("%d"), &nParamPairs);
	for (auto i = 0; i < nParamPairs; i++)
	{
		int param;
		std::string value;
		GetNextLine(config, buffer, 199);
		_stscanf(buffer, _T("%d %s"), &param, &value);
		SaveConfigParam(param, value);
	}

	// There should be 8 pairs of row and col info
	for (auto i = 0; i < 8; i++)
	{
		GetNextLine(config, buffer, 199);
		_stscanf(buffer, _T("%d %d"), &m_rows[i], &m_cols[i]);
	}

	if (m_layerMode == TWO_OR || m_layerMode == TWO_XOR)
	{
		for (auto i = 0; i < 8; i++)
		{
			GetNextLine(config, buffer, 199);
			_stscanf(buffer, _T("%d %d"), &layerPairValues[i][0], &layerPairValues[i][1]);
		}
	}
	UpdateData(FALSE);

	// Process special Parameters
	ProcessConfigParameters();

	return true;
}


void CCheckerBoardImageDlg::ProcessConfigParameters()
{
	vector<ConfigParameter>::iterator it;
	for (it = configParameters.begin(); it != configParameters.end(); it++) 
	{
		switch (it->m_paramId)
		{
		case COLORTABLE:
			CString cs = CString(it->m_strValue.c_str());
			ReadColorTable(cs);
			break;
		}
	}
}


void CCheckerBoardImageDlg::SaveConfigParam(int param, int value)
{
	ConfigParameter p(param, value);
	configParameters.push_back(p);
}

void CCheckerBoardImageDlg::SaveConfigParam(int param, std::string value)
{
	ConfigParameter p(param, value);
	configParameters.push_back(p);
}



// Come here anytime one of the radio buttons for
// SIngle or OR or XOR is pressed
void CCheckerBoardImageDlg::OnBnClickedMode()
{
	UpdateData();
	// TODO: Add your control notification handler code here
	ShowLayerPairs(m_layerMode != SINGLE_LAYER);
}

// Only visible when 2 sub layers are Combined (OR or XOR) 
// to layer
void CCheckerBoardImageDlg::ShowLayerPairs(BOOL show)
{
	m_layerPairBox.ShowWindow(show);
	for (auto i = 0; i < 8; i++)
	{
		m_layerPair[i][0].ShowWindow(show);
		//m_layerPair[i][1].ShowWindow(show);
	}
}


void CCheckerBoardImageDlg::OnBnClickedLoadConfig()
{
	LoadConfiguration();
}

// Takes m_height or m_widt (or any other integer)
// and creates vector of all divisors of that number
void CCheckerBoardImageDlg::GetDivisors(int n, std::vector<int> *divisors)
{
	// Clean out previous values from Divisor Lists
	divisors->clear();

	// Vector to store half of the divisors 
	vector<int> low, high;
	int sqrtn = (int) sqrt(n);
	for (int i = 2; i <= sqrtn; i++)
	{
		if (n%i == 0)
		{
			if (n / i == i) // check if divisors are equal 
				low.push_back(i);
			else
			{
				low.push_back(i);

				// push the second divisor in the vector 
				high.push_back(n / i);
			}
		}
	}

	// Copy to divisors vector
	divisors->push_back(1);
	vector<int>::iterator it;
	for (it = low.begin(); it != low.end(); it++)
		divisors->push_back(*it);
	// Add high values backwards
	for (auto it = high.rbegin(); it != high.rend(); it++)
		divisors->push_back(*it);
}

void CCheckerBoardImageDlg::OnEnChangeWidth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	//CreateRowColDropLists();
	//OnBnClickedRandom();
}


void CCheckerBoardImageDlg::OnEnChangeHeight()
{
	CreateRowColDropLists();
	OnBnClickedRandom();
}

// Puts data in Drop Lists into m_rows and m_cols arrays
void CCheckerBoardImageDlg::ReadRowColDropLists()
{
	for (auto i = 0; i < 8; i++)
	{
		m_rows[i] = m_comboRow[i].GetItemData(m_comboRow[i].GetCurSel());
		m_cols[i] = m_comboCol[i].GetItemData(m_comboCol[i].GetCurSel());
	}
}

// Set the Drop Down Lists for Rows and Cols to Values
void CCheckerBoardImageDlg::SetRowColDropLists()
{
	TCHAR s[10];
	LPCTSTR  str;
	for (auto i = 0; i < 8; i++)
	{
		str = _itot(m_rows[i], s, 10);
		m_comboRow[i].SelectString(-1, str);	// -1 means search list from beginning

		str = _itot(m_cols[i], s, 10);
		m_comboCol[i].SelectString(-1, str);
	}
}

// Ignore return - usually closes dialog
void CCheckerBoardImageDlg::OnOK()
{
	CWnd* pWnd = GetFocus();
	if (GetDlgItem(IDOK) == pWnd)
	{
		CDialog::OnOK();
		return;
	}
}

void CCheckerBoardImageDlg::OnEnKillfocusWidth()
{
	CreateRowColDropLists();
	OnBnClickedRandom();
}


void CCheckerBoardImageDlg::OnBnClickedTest()
{
	Quadrilateral qd;
	qd.Construct(40, 20, 1,  1);
	qd.Draw();
	qd.Show();


	//EllipseBoard el;
	//int wid = 10;
	//int hgt = 10;
	//el.Construct(wid, hgt, 1, 1);
	////el.Construct(wid, hgt);
	////el.m_columns = 1;
	////el.m_rows = 1;
	//el.UL_Q(0, 0, wid, hgt);
	//el.Show();
	////bm.FlipVert(&el, 0, 0, 0, 0, wid, hgt);
	//bm.FlipHoriz(&el, 0, 0, 0, 0, wid, hgt);
	//bm.Show();



	/////////////////////////////////////////////////////
	//TriangleBoard t(10, 10, 1, 1);
	//t.Draw();
	//t.Show();

	//BooleanBitmap b;
	//b.Construct(10, 10);
	////b.Copy(&t, 0, 0, 0, 0, 10, 10);
	////b.Show();

	//b.RotateCounter(&t, 0, 0, 0, 0, 10);
	//b.Show();
	//EllipseBoard e(30, 30, 1, 1);
	//e.Draw();
	//e.Show();

	///////////////////////////////////////////////////////
	//int n = TriDist(2, 2);
	////EllipseBoard t(10, 10, 1, 1, FALSE);
	////t.Show();
	////delete &t;	
	//EllipseBoard u(30, 40, 1, 1, FALSE);
	//u.Show();
	////delete &u; 
	//EllipseBoard v(80, 30, 1, 1, FALSE);
	//v.Show();
	////delete &v;
//	std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
//	std::vector<int> v2 = {};
//	int n = v.size();
//	for (auto i = 1; i < v.size(); i++)
//	{
//		v2.push_back(v[i]);
//	}
//	int n2 = v2.size();
//
//	char buffer[100];
//
//	for (auto i = 0; i < 8; i++)
//	{
//		int i_ht = i%n2;
//		sprintf_s(buffer, "i %d  ix %d   val %d\n", i, i_ht, v2[i_ht]);
//		OutputDebugStringA(buffer);
//
//	}
//	OnBnClickedLayoutReset();
//#define O()  file.Write((LPCTSTR)str, strlen(str))
//	char str[200];
//
//	CString szPathName = _T("K:\\Users\\Steve\\CSUSM - ArtProject\\foo.txt");
//	//CFile::Remove(szPathName);
//
//	// Open File
//	CStdioFile file;
//	UINT flags = CFile::modeCreate | CFile::modeWrite;// | CFile::typeText;
//	CFileException e;
//	try
//	{
//		BOOL res = file.Open(szPathName, flags, &e);
//		sprintf_s(bfr, "File created:  %s  Ret Val %d\n", szPathName, res);//O();
//		OutputDebugStringA(bfr);
//	}
//	catch (...)
//	{
//		CString err;
//		err.GetBuffer(100);
//		e.GetErrorMessage(err.GetBuffer(), 100);
//		AfxMessageBox(err);
//		return;// false;
//	}
//
//	//////////////////////////////////////////////////////////////
//	//// Ready to write
//
//	
//	//sprintf_s(str, "%s\n\0", szPathName); O();
//	char* colorTable = CStringToCharStar(szPathName);
//	sprintf_s(str, "%s\n\0", colorTable); O();
//	file.Close();
//
//
//
//	//////// Read file ///////////////////////////////
//	// Open File
//	TCHAR buffer[200];
//	FILE *config;
//	config = _tfopen(szPathName, _T("r"));
//
//	CString cs;
//
//	GetNextLine(config, buffer, 199);
//
//	//_stscanf(buffer, _T("%s"), &cs);
//	//sprintf_s(bfr, "_stscanf(buffer, _T(\"%s\"), CString);\n", cs);//O();
//	//OutputDebugStringA(bfr);
//
//	char* chars[200];
//
//	_stscanf(buffer, _T("%s"), &chars);
//	sprintf_s(bfr, "_stscanf(buffer, _T(\"%s\"), char[]);\n", chars);//O();
//	OutputDebugStringA(bfr);
//
//	std::string value;
//	_stscanf(buffer, _T("%s"), &value);
//	sprintf_s(bfr, "_stscanf(buffer, _T(\"%s\"), std::string);\n", value);//O();
//	OutputDebugStringA(bfr);
//
//	file.Close();
}


void CCheckerBoardImageDlg::OnBnClickedNextimage()
{
	// TODO: Add your control notification handler code here
	nextImageClicked = true;
	OnBnClickedCreateImage();
	nextImageClicked = false;
}


void CCheckerBoardImageDlg::OnBnClickedRandomPairs()
{
	RandomLayerPairs();
}


void CCheckerBoardImageDlg::OnBnClickedLayoutReset()
{
	char buffer[100];
	// Ignore divisor of 1
	// length of vectors may be less than 8
	//int n_ht = heightDivisors.size();
	//int n_wd = widthDivisors.size();

	// Create new vectors that excludes first entry (which is always 1)
	std::vector<int> hgt;
	std::vector<int> wid;
	for (auto i = 1; i < (int)heightDivisors.size(); i++)
		hgt.push_back(heightDivisors[i]);
	for (auto i = 1; i < (int)widthDivisors.size(); i++)
		wid.push_back(widthDivisors[i]);
	int n_hgt = hgt.size();
	int n_wid = wid.size();

	for (auto i = 0; i < 8; i++)
	{
		m_rows[i] = hgt[i%n_hgt];
		m_cols[i] = wid[i%n_wid];
		sprintf_s(buffer, "i %d  rows %d  cols %d\n", i, m_rows[i], m_cols[i]);
		OutputDebugStringA(buffer);
	}
	SetRowColDropLists();
	UpdateData(FALSE);
}


void CCheckerBoardImageDlg::OnBnClickedResetPairs()
{
	// 0 1
	// 1 2
	// ...
	// 7 0
	for (auto i=0; i<8; i++ )
	{ 
		layerPairValues[i][0] = (i + 1) % 8;
		layerPairValues[i][1] = (i+1)%8;
	}
	UpdateData(FALSE);
}


void CCheckerBoardImageDlg::OnBnClickedCheckRandom()
{
	// If the random box is not checked, 
	// then the Refresh Seed check box is not relevant
	UpdateData(TRUE);
	m_refreshRandomCtrl.EnableWindow(m_random == TRUE);
}




