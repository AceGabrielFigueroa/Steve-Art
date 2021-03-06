// EnhancedCreatePaletteDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "MFCApplication1.h"
#include "EnhancedCreatePaletteDlg.h"
#include "Resource.h"
#include "ColorPalette.h"
#include "afxdialogex.h"
#include "CRGB.h"
#include "CHSV.h"
#include "CPict.h"
#include <iostream>
#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// EnhancedCreatePaletteDlg dialog

EnhancedCreatePaletteDlg::EnhancedCreatePaletteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CREATEPALETTE_DIALOG, pParent)
	, m_hue_only(FALSE)
	, m_limit_sliders(FALSE)
	//, m_evenColors(FALSE)
	//, m_radioColorOption(3)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void EnhancedCreatePaletteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_HUEINC, m_hueIncSlider);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_brightnessSlider);
	DDX_Control(pDX, IDC_SLIDER_SATURATION, m_saturationSlider);

	DDX_Control(pDX, IDC_STATIC_PALETTE, m_palette);
	DDX_Control(pDX, IDC_SAVECOLORTABLE, m_saveColorTable);

	DDX_Control(pDX, IDC_BUTTON0, m_clr[0]);
	DDX_Control(pDX, IDC_BUTTON1, m_clr[1]);
	DDX_Control(pDX, IDC_BUTTON2, m_clr[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_clr[3]);
	DDX_Control(pDX, IDC_BUTTON0_SAT, m_clrSat[0]);
	DDX_Control(pDX, IDC_BUTTON1_SAT, m_clrSat[1]);
	DDX_Control(pDX, IDC_BUTTON2_SAT, m_clrSat[2]);
	DDX_Control(pDX, IDC_BUTTON3_SAT, m_clrSat[3]);
	DDX_Check(pDX, IDC_CHECK_HUE_ONLY, m_hue_only);
	DDX_Check(pDX, IDC_CHECK_LIMIT_SLIDERS, m_limit_sliders);
	DDX_Radio(pDX, IDC_RADIO1, m_radioColorOption);
}

BEGIN_MESSAGE_MAP(EnhancedCreatePaletteDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SAVECOLORTABLE, &EnhancedCreatePaletteDlg::OnBnClickedSavecolortable)
	ON_BN_CLICKED(IDCLOSE, &EnhancedCreatePaletteDlg::OnBnClickedClose)

	ON_BN_CLICKED(IDC_BUTTON0, &EnhancedCreatePaletteDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON1, &EnhancedCreatePaletteDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &EnhancedCreatePaletteDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &EnhancedCreatePaletteDlg::OnBnClickedButton3)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_HUEINC, &EnhancedCreatePaletteDlg::OnNMCustomdrawSliderHueinc)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SATURATION, &EnhancedCreatePaletteDlg::OnNMCustomdrawSliderSaturation)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BRIGHTNESS, &EnhancedCreatePaletteDlg::OnNMCustomdrawSliderBrightness)

	ON_BN_CLICKED(IDC_CHECK_HUE_ONLY, &EnhancedCreatePaletteDlg::OnBnClickedCheckHueOnly)
	ON_BN_CLICKED(IDC_CHECK_LIMIT_SLIDERS, &EnhancedCreatePaletteDlg::OnBnClickedCheckLimitSliders)
	//ON_BN_CLICKED(IDC_CHECK_EVEN_COLOR, &EnhancedCreatePaletteDlg::OnBnClickedCheckEvenColor)

	ON_BN_CLICKED(IDC_RADIO1, &EnhancedCreatePaletteDlg::OnBnClickedColorOption)
	ON_BN_CLICKED(IDC_RADIO2, &EnhancedCreatePaletteDlg::OnBnClickedColorOption)
	ON_BN_CLICKED(IDC_RADIO3, &EnhancedCreatePaletteDlg::OnBnClickedColorOption)
	ON_BN_CLICKED(IDC_RADIO4, &EnhancedCreatePaletteDlg::OnBnClickedColorOption)
	ON_BN_CLICKED(IDC_RADIO_COMPLIMENTARY, &EnhancedCreatePaletteDlg::OnBnClickedColorOption)
	ON_BN_CLICKED(IDC_RADIO_TRIANGLE, &EnhancedCreatePaletteDlg::OnBnClickedColorOption)
	ON_BN_CLICKED(IDC_RADIO_SQUARE, &EnhancedCreatePaletteDlg::OnBnClickedColorOption)
	ON_BN_CLICKED(IDC_BUTTON4, &EnhancedCreatePaletteDlg::OnTest)
END_MESSAGE_MAP()


// EnhancedCreatePaletteDlg message handlers

BOOL EnhancedCreatePaletteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

	m_palette.Invalidate();
	m_palette.pParentDialog = this;

	cm.Init();	// Initialize the ColorModel

	InitColorControls();
	InitSliders();
	m_saveColorTable.EnableWindow(FALSE);
	//colorsHaveChanged = false;
	paletteCreated = false;
	m_hue_only = TRUE;
	cm.SetHueOnly(true);
	m_limit_sliders = TRUE;
	cm.SetLimitRange(true);

	m_radioColorOption = TWO;	// Default is 4 unrestricted colors
	cm.SetColorOption(TWO);
	UpdateData(FALSE);
	EnableColorControls();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void EnhancedCreatePaletteDlg::InitSliders()
{
	const int SLIDERLIMIT = 100;
	const int INIT_HUEINC = 65;
	const int INIT_SATURATION = 65;
	const int INIT_BRIGHTNESS = 65;
	m_saturationSlider.SetRange(0, SLIDERLIMIT);
	m_hueIncSlider.SetRange(0, SLIDERLIMIT);
	m_brightnessSlider.SetRange(0, SLIDERLIMIT);

	m_saturationSlider.SetPos(INIT_SATURATION);
	cm.SetSaturationRange(INIT_SATURATION);

	m_brightnessSlider.SetPos(INIT_BRIGHTNESS);
	cm.SetValueRange(INIT_BRIGHTNESS);

	m_hueIncSlider.SetPos(INIT_HUEINC);
	cm.SetHueIncRange(INIT_HUEINC);

	savedHueInc = 0;
	savedSaturation = 0;
	savedBrightness = 0;
}

void EnhancedCreatePaletteDlg::InitColorControls()
{
	//InitColorControl(0,   m_clr[0], m_clrSat[0], clrs[0]);
	//InitColorControl(90,  m_clr[1], m_clrSat[1], clrs[1]);
	//InitColorControl(180, m_clr[2], m_clrSat[2], clrs[2]);
	//InitColorControl(270, m_clr[3], m_clrSat[3], clrs[3]);	
	InitColorControl(0, 0);
	InitColorControl(1, 90);
	InitColorControl(2, 180);
	InitColorControl(3, 270);

	// These are checked against current values when "Close" 
	// is pressed to see if Palette was not saved
	savedClrs[0] = 0;
	savedClrs[1] = 0;
	savedClrs[2] = 0;
	savedClrs[3] = 0;
}

//void EnhancedCreatePaletteDlg::InitColorControl(int hue, CMFCButton &clr, CMFCButton &clrSat, COLORREF &colorRef)
void EnhancedCreatePaletteDlg::InitColorControl(int colorNumber, int hue)
{
	//clr.EnableWindowsTheming(FALSE);
	//clr.m_bTransparent = FALSE;
	//clrSat.EnableWindowsTheming(FALSE);
	//clrSat.m_bTransparent = FALSE;

	//CHSV hsv(hue, 1.0, 1.0);
	//CRGB rgb = hsv.HSVToRGB();
	//COLORREF initColorRef = rgb.GetColorRef();
	//clr.SetFaceColor(initColorRef);
	//clrSat.SetFaceColor(initColorRef);
	//colorRef = initColorRef;

	m_clr[colorNumber].EnableWindowsTheming(FALSE);
	m_clr[colorNumber].m_bTransparent = FALSE;
	m_clrSat[colorNumber].EnableWindowsTheming(FALSE);
	m_clrSat[colorNumber].m_bTransparent = FALSE;

	CHSV hsv(hue, 1.0, 1.0);
	CRGB rgb = hsv.HSVToRGB();
	COLORREF initColorRef = rgb.GetColorRef();
	m_clr[colorNumber].SetFaceColor(initColorRef);
	m_clrSat[colorNumber].SetFaceColor(initColorRef);
	cm.SetColorRef(colorNumber, initColorRef);

	//TRACE(_T("\nInitColorControl - RGB value of the selected color - red = %u, ")
	//	_T("green = %u, blue = %u\n"), rgb.R, rgb.G, rgb.B );
	char str[256];
	sprintf_s(str, "InitColorControl - color %d - Ref %6x  R %3d  G %3d  B %3d\n",
		colorNumber, initColorRef, rgb.R, rgb.G, rgb.B);
		
	OutputDebugStringA(str);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void EnhancedCreatePaletteDlg::OnPaint()
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
HCURSOR EnhancedCreatePaletteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// CreatePallette
void EnhancedCreatePaletteDlg::DrawPalette()
{
	UpdateData();

	cm.GeneratePalette();
	DisplayPalette();
	paletteCreated = true;

	m_saveColorTable.EnableWindow(TRUE);	// Enable the Save Color Table Button
}

void EnhancedCreatePaletteDlg::EndDialog(int nresult)
{
	CDialogEx::EndDialog(nresult);
}

//int EnhancedCreatePaletteDlg::hueFromColorRef(COLORREF colorRef)
//{
//	CRGB c(colorRef);
//	CHSV hsv = c.RGBToHSV();
//
//	return (int) hsv.H;
//}
//
//double EnhancedCreatePaletteDlg::saturationFromColorRef(COLORREF colorRef)
//{
//	CRGB c(colorRef);
//	CHSV hsv = c.RGBToHSV();
//
//	return hsv.S;
//}
//
//double EnhancedCreatePaletteDlg::valueFromColorRef(COLORREF colorRef)
//{
//	CRGB c(colorRef);
//	CHSV hsv = c.RGBToHSV();
//
//	return hsv.V;
//}

// We do this to calculate the values of the hue increments.
// The user can choose up to 4 hues.  
// The hue increments are based on the distance around the color wheel of 
// each hue to the next one.
// Since the user can specify the colors in any order (with respect to 
// placement on the circle of hues), we need to sort them to do the calculation
void EnhancedCreatePaletteDlg::SortColorControls()
{
	//for (auto i = 0; i < 4; i++)
	//	m_hues[i] = hueFromColorRef(clrs[i]);

	//for (auto i = 0; i < 3; i++)
	//	for (auto j = i + 1; j < 4; j++)
	//		if (m_hues[j] < m_hues[i])
	//		{
	//			int hue = m_hues[j];
	//			m_hues[j] = m_hues[i];
	//			m_hues[i] = hue;

	//			COLORREF ref = clrs[j];
	//			clrs[j] = clrs[i];
	//			clrs[i] = ref;
	//		}
	cm.SortColors();
	//
	// Reset Color Controls to sorted values

	//SetColorControl(cm.GetColorRef(0), m_clr[0], m_clrSat[0]);
	//SetColorControl(cm.GetColorRef(1), m_clr[1], m_clrSat[1]);
	//SetColorControl(cm.GetColorRef(2), m_clr[2], m_clrSat[2]);
	//SetColorControl(cm.GetColorRef(3), m_clr[3], m_clrSat[3]);
	
	//char str[222];
	//sprintf_s(str, "SortColorControls ********\n");
	OutputDebugStringA("SortColorControls ********\n");
	
	SetColorButton(0);
	SetColorButton(1);
	SetColorButton(2);
	SetColorButton(3);
}

//void EnhancedCreatePaletteDlg::SetColorControl(COLORREF clrRef, CMFCButton &clr, CMFCButton &clrSat)
//{
//	CRGB rgb(clrRef);
//	CHSV hsv = rgb.RGBToHSV();
//	hsv.S = 1.0;
//	hsv.V = 1.0;
//	CRGB rgbSat = hsv.HSVToRGB();
//	COLORREF satcolor = rgbSat.GetColorRef();
//
//	clr.SetFaceColor(clrRef, TRUE);
//	clrSat.SetFaceColor(satcolor, TRUE);
//}


void EnhancedCreatePaletteDlg::DisplayPalette()
{
	m_palette.ClearCellRects();	// clear out the previous palette

	// calculate where the elements of the palette will be located
	RECT clientRect;
	GetClientRect(&clientRect);

	int top = clientRect.top;
	int left = clientRect.left;

	// set the size of the palette elements
	int space = paletteSquareSpc;
	int width = paletteSquareWid;
	int height = paletteSquareHgt;

	CRGB* RGBpalette = cm.GetPalette();

	for (int i = 0; i < 256; i++)
	{
		int row = i % 16;
		int col = i >> 4;
		int squareTop = top + row * (height + space);
		int squareLeft = left + col * (width + space);

		CRGB rgb = RGBpalette[i];
		COLORREF color = rgb.GetColorRef();

		m_palette.AddCellRect(squareLeft, squareTop, width, height, color);
	}
	m_palette.Invalidate();
}

void EnhancedCreatePaletteDlg::OnBnClickedSavecolortable()
{
	CFileDialog dlgFile(FALSE, CString(".txt"), NULL, OFN_OVERWRITEPROMPT, CString("Text Files (*.txt)|*.txt|"));

	CString fileName;

	if (dlgFile.DoModal() == IDOK)
	{
		fileName = dlgFile.GetPathName();
	}
	else
	{
		AfxMessageBox(_T("Cannot Save Color Table"));
		return;
	}

	cm.SaveColorTable(fileName);

	//CStdioFile file;
	//UINT flags = CFile::modeCreate | CFile::modeWrite;// | CFile::typeText;

	//CFileException e;
	//try
	//{
	//	file.Open(fileName, flags, &e);
	//}
	//catch (...)
	//{
	//	CString err;
	//	err.GetBuffer(100);
	//	e.GetErrorMessage(err.GetBuffer(), 100);
	//	AfxMessageBox(err);
	//	return;
	//}

	////// Ready to write
	//char str[200];

	//cm.GeneratePalette();

	//// Safety Value - check this when we read it to avoid reading an inappropriate file
	//sprintf_s(str, "%d\n\0", SAFETY_VALUE);
	//file.Write((LPCTSTR)str, strlen(str));

	//// SAve Parameters used to create color tabnle as comments 
	//
	////// Write 4 Hues
	////sprintf_s(str, "# Hues %d %d %d %d\n#\n\0", m_hues[0], m_hues[1], m_hues[2], m_hues[3]);
	////file.Write((LPCTSTR)str, strlen(str));
	//
	////// Write 4 Hue Increments
	////sprintf_s(str, "# HueInc %d %d %d %d\n#\n\0", m_hueInc[0], m_hueInc[1], m_hueInc[2], m_hueInc[3]);
	////file.Write((LPCTSTR)str, strlen(str));
	//
	////// Write 4 Saturation
	////sprintf_s(str, "# Saturation %d %d %d %d\n#\n\0", m_saturation[0], m_saturation[1], m_saturation[2], m_saturation[3]);
	////file.Write((LPCTSTR)str, strlen(str));
	//
	////// Write 4 Brightness
	////sprintf_s(str, "# Brightness %d %d %d %d\n#\n\0", m_brightness[0], m_brightness[1], m_brightness[2], m_brightness[3]);
	////file.Write((LPCTSTR)str, strlen(str));
	//
	//// Now write out color Data
	//CRGB* RGBpalette = cm.GetPalette();
	//
	//strcpy_s(str, "# Colors  Index  Red  Green Blue\n\0");
	//file.Write((LPCTSTR)str, strlen(str));
	//
	//for (int i = 0; i < 256; i++)
	//{
	//	CRGB rgb = RGBpalette[i];
	//	
	//	sprintf_s(str, "%d %d %d %d\n\0", i, rgb.R, rgb.G, rgb.B);
	//	file.Write((LPTSTR)str, strlen(str));
	//}

	//file.Close();

	SavePaletteImage(fileName);

	m_saveColorTable.EnableWindow(FALSE);
	//colorsHaveChanged = false;

	savedClrs[0] = cm.GetColorRef(0);
	savedClrs[1] = cm.GetColorRef(1);
	savedClrs[2] = cm.GetColorRef(2);
	savedClrs[3] = cm.GetColorRef(3);
	savedHueInc = m_hueIncSlider.GetPos();
	savedSaturation = m_saturationSlider.GetPos();
	savedBrightness = m_brightnessSlider.GetPos();
}

void EnhancedCreatePaletteDlg::SavePaletteImage(CString fileName)
{
	///////////////////////////////////////////////
	// Fix File Name
	// remove .txt and add .bmp
	CString bmp = _T(".bmp");
	CString szPathName;
	int loc = fileName.Find(_T(".txt"));
	if (loc > 0)
	{
		szPathName = fileName.Left(fileName.GetLength() - 4) + bmp;
	}
	else
	{
		szPathName = fileName + bmp;
	}

	// Need to convert string type
	char imageFileName[200];
	for (int i = 0; i < szPathName.GetLength(); i++)
	{
		imageFileName[i] = (char)szPathName.GetAt(i);
	}
	imageFileName[szPathName.GetLength()] = 0;

	///////////////////////////////////////////////////////
	// Save Image
	m_palette.SaveDCToBitmap(imageFileName);
}

boolean EnhancedCreatePaletteDlg::ColorsHaveChanged()
{
	return !
		(savedClrs[0] == cm.GetColorRef(0) &&
		savedClrs[1] == cm.GetColorRef(0) &&
		savedClrs[2] == cm.GetColorRef(0) &&
		savedClrs[3] == cm.GetColorRef(0) &&
		savedHueInc == m_hueIncSlider.GetPos() &&
		savedSaturation == m_saturationSlider.GetPos() &&
		savedBrightness == m_brightnessSlider.GetPos());
}

void EnhancedCreatePaletteDlg::OnBnClickedClose()
{
	// Show the Message Dialog Box if a Palette has been created and 
	// current colors are not the same as the colors at the last save
	boolean changed = ColorsHaveChanged();
	if (!paletteCreated || !changed)
	{
		EndDialog(0);
	}
	else
	if ( AfxMessageBox(_T("Color Table Not Saved.\nClose Dialog?"), MB_OKCANCEL) == IDOK)
	{
		EndDialog(0);
	}
}

void EnhancedCreatePaletteDlg::OnNMCustomdrawSliderHueinc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	cm.SetHueIncRange(m_hueIncSlider.GetPos());
	DrawPalette();

	*pResult = 0;
}

void EnhancedCreatePaletteDlg::OnNMCustomdrawSliderSaturation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	cm.SetSaturationRange(m_saturationSlider.GetPos());
	DrawPalette();
	
	*pResult = 0;
}

void EnhancedCreatePaletteDlg::OnNMCustomdrawSliderBrightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	cm.SetValueRange(m_brightnessSlider.GetPos());
	DrawPalette();

	*pResult = 0;
}

void EnhancedCreatePaletteDlg::OnBnClickedButton0()
{
	//ClickColorButton(clrs[0], m_clr1, m_clr1sat);
	//ClickColorButton(clrs[0], m_clr[0], m_clrSat[0]);
	ClickColorButton(0);
}

void EnhancedCreatePaletteDlg::OnBnClickedButton1()
{
	//ClickColorButton(clrs[1], m_clr2, m_clr2sat);
	//ClickColorButton(clrs[1], m_clr[1], m_clrSat[1]);
	ClickColorButton(1);
}

void EnhancedCreatePaletteDlg::OnBnClickedButton2()
{
	//ClickColorButton(clrs[2], m_clr3, m_clr3sat);
	//ClickColorButton(clrs[2], m_clr[2], m_clrSat[2]);
	ClickColorButton(2);
}

void EnhancedCreatePaletteDlg::OnBnClickedButton3()
{
	//ClickColorButton(clrs[3], m_clr4, m_clr4sat);
	//ClickColorButton(clrs[3], m_clr[3], m_clrSat[3]);
	ClickColorButton( 3 );
}

//void EnhancedCreatePaletteDlg::ClickColorButton(COLORREF &clrRef, CMFCButton &clr, CMFCButton &clrSat)
void EnhancedCreatePaletteDlg::ClickColorButton(int colorNumber)

{
	char str[256];
	COLORREF clrRef = cm.GetColorRef(colorNumber);

	sprintf_s(str, "ClkColorButton -- Orig color %d  Ref %6x, R %3d G %3d B %3d\n", 
		colorNumber, clrRef, GetRValue(clrRef), GetGValue(clrRef), GetBValue(clrRef));
	OutputDebugStringA(str);

	CColorDialog dlg(clrRef, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		COLORREF dlgColor = dlg.GetColor();
		sprintf_s(str, "ClkColorButton -- New  color %d  Ref %6x, R %3d G %3d B %3d\n",
			colorNumber, dlgColor, GetRValue(dlgColor), GetGValue(dlgColor), GetBValue(dlgColor));
		OutputDebugStringA(str);


		//CRGB rgb(GetRValue(dlgColor), GetGValue(dlgColor), GetBValue(dlgColor));
		//CHSV hsv = rgb.RGBToHSV();
		//hsv.S = 1.0;
		//hsv.V = 1.0;
		//CRGB rgbSat = hsv.HSVToRGB();
		//COLORREF satcolor = rgbSat.GetColorRef();

		//m_clr[colorNumber].SetFaceColor(dlgColor, TRUE);
		//m_clrSat[colorNumber].SetFaceColor(satcolor, TRUE);
		cm.SetColorRef(colorNumber, dlgColor);
		SetColorButton(colorNumber, dlgColor);

		// AA SortColorControls();

		//colorsHaveChanged = true;
		DrawPalette();
	}
}

void EnhancedCreatePaletteDlg::SetColorButton(int colorNumber, COLORREF cr)
{
	//CRGB rgb(GetRValue(cr), GetGValue(cr), GetBValue(cr));
	CRGB rgb(cr);
	//COLORREF cr = cm.GetColorRef(colorNumber);
	CHSV hsv = rgb.RGBToHSV();
	hsv.S = 1.0;
	hsv.V = 1.0;
	CRGB rgbSat = hsv.HSVToRGB();
	COLORREF satcolor = rgbSat.GetColorRef();

	m_clr[colorNumber].SetFaceColor(cr, TRUE);
	m_clrSat[colorNumber].SetFaceColor(satcolor, TRUE);

	char str[256];
	sprintf_s(str, "SetColorButton --      color %d  Ref %6x, R %3d G %3d B %3d  Hue %d\n", 
		colorNumber, cr, GetRValue(cr), GetGValue(cr), GetBValue(cr), (int) hsv.H );
	OutputDebugStringA(str);

	UpdateData(FALSE);
}

void EnhancedCreatePaletteDlg::SetColorButton(int colorNumber )
{
	//CRGB rgb(GetRValue(cr), GetGValue(cr), GetBValue(cr));
	//CRGB rgb(cr);
	COLORREF cr = cm.GetColorRef(colorNumber);
	char str[256];
	sprintf_s(str, "SetColorButton --      color %d  Ref %6x\n",	colorNumber, cr);
	OutputDebugStringA(str);
	SetColorButton(colorNumber, cr );
}

void EnhancedCreatePaletteDlg::OnBnClickedCheckHueOnly()
{
	UpdateData(TRUE);
	cm.SetHueOnly(m_hue_only);
	DrawPalette();
}

void EnhancedCreatePaletteDlg::OnBnClickedCheckLimitSliders()
{
	UpdateData(TRUE);
	cm.SetLimitRange(m_limit_sliders);
	DrawPalette();
}

void EnhancedCreatePaletteDlg::EnableColorControls()
{
	UpdateData();

	// All 4 colors are used for each option, but for some cases, they are 
	// determined by the algorithm
	// For some, they are calculated by the algorithm and are hidden from the user

	switch (m_radioColorOption)
	{
	case ONE:
	case COMPLIMENTARY:
	case TRIANGLE:
	case SQUARE:
		// These four cases only have one color selection control enabled
		m_clr[1].ShowWindow(FALSE);  m_clrSat[1].ShowWindow(FALSE);
		m_clr[2].ShowWindow(FALSE);  m_clrSat[2].ShowWindow(FALSE);
		m_clr[3].ShowWindow(FALSE);  m_clrSat[3].ShowWindow(FALSE);
		break;
	case TWO:
		m_clr[1].ShowWindow(FALSE);	m_clrSat[1].ShowWindow(FALSE);
		m_clr[2].ShowWindow(TRUE);	m_clrSat[2].ShowWindow(TRUE);
		m_clr[3].ShowWindow(FALSE);	m_clrSat[3].ShowWindow(FALSE);
		break;
	case THREE:
		m_clr[1].ShowWindow(FALSE);	m_clrSat[1].ShowWindow(FALSE);
		m_clr[2].ShowWindow(TRUE);	m_clrSat[2].ShowWindow(TRUE);
		m_clr[3].ShowWindow(TRUE);	m_clrSat[3].ShowWindow(TRUE);
		break;
	case FOUR:
		m_clr[1].ShowWindow(TRUE);	m_clrSat[1].ShowWindow(TRUE);
		m_clr[2].ShowWindow(TRUE);	m_clrSat[2].ShowWindow(TRUE);
		m_clr[3].ShowWindow(TRUE);	m_clrSat[3].ShowWindow(TRUE);
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	SetColorButton(0);
	SetColorButton(1);
	SetColorButton(2);
	SetColorButton(3);
	UpdateData(FALSE);
}


void EnhancedCreatePaletteDlg::OnBnClickedColorOption()
{
	UpdateData();
	cm.SetColorOption(m_radioColorOption);
	EnableColorControls();
	DrawPalette();
}

void EnhancedCreatePaletteDlg::OnTest()
{
	CHSV hsv1(200.0, 0.5, 0.5);
	CRGB rgb1 = hsv1.HSVToRGB();
	COLORREF cr1 = rgb1.GetColorRef();

	CHSV hsv2(100.0, 0.9, 0.7);
	CRGB rgb2 = hsv2.HSVToRGB();
	COLORREF cr2 = rgb2.GetColorRef();

	cm.CopySatValFromColorRefToColorRef(cr1, cr2);

	CRGB newrgb(cr2);
	CHSV newhsv = newrgb.RGBToHSV();
}
