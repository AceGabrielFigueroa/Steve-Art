// EnhancedCreatePaletteDlg.h : header file
//
#include "CHSV.h"
#include "CRGB.h"
#include "CPict.h"
#include "ColorModel.h"
#pragma once


class CRGB;
//class CHSV;


// EnhancedCreatePaletteDlg dialog
class EnhancedCreatePaletteDlg : public CDialogEx
{
// Construction
public:
	EnhancedCreatePaletteDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATEPALETTE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	const int paletteSquareHgt = 24;
	const int paletteSquareWid = 24;
	const int paletteSquareSpc = 2;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	void EndDialog(int nresult);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	ColorModel cm;
	enum COLOROPTION { ONE, TWO, THREE, FOUR, COMPLIMENTARY, TRIANGLE, SQUARE};

	// Current Values
	//COLORREF clrs[4];	//ColorModel

	// Values at last Save
	COLORREF savedClrs[4];
	int savedHueInc;
	int savedSaturation;
	int savedBrightness;

	boolean paletteCreated;
	
	CMFCButton m_clrSat[4];
	CMFCButton m_clr[4];

	CSliderCtrl m_hueIncSlider;
	CSliderCtrl m_saturationSlider;	
	CSliderCtrl m_brightnessSlider;
	CPict m_palette;
	CButton m_saveColorTable;
	//CButton m_close;

private:
	// These three constants must be in the range of 1 to 100
	// The defaults are what sunbjectively seems the best choice, 
	// but we let the user overridr them.
	//const int MIN_SATURATION = 20;
	//const int MIN_BRIGHTNESS = 65;
	//const int MAX_HUE_INCREMENT = 50;
	//const int SAFETY_VALUE = 314159265;

	//int m_hues[4];			//ColorModel
	//int m_hueInc[4][4];		//ColorModel

	//CRGB RGBpalette[256];	//ColorModel
	BOOL m_hue_only;		//ColorModel
	BOOL m_limit_sliders;	//ColorModel
							// When true (ie checked) the max value of each of
							// the HueInc, Sat and Value sliders is restricted to what
							// "practical" values.  
							// Ie, ones that give subjectively pleasant palettes
							// User can overrid this by unchecking
	int m_radioColorOption;	//ColorModel

	////////////////////////
	//	ColorModel
	//int hueFromColorRef(COLORREF colorRef); 
	//double saturationFromColorRef(COLORREF colorRef);
	//double valueFromColorRef(COLORREF colorRef);
	//boolean GeneratePalette();
	//CRGB* GetPalette();
	//void CalculateHuesAndIncrements();
	//double CalculateHueSpread(int hueA, int hueB, boolean shoudBePositive);
	//void SetHueInc(int hueIx, double hueInc);
	//double CalculateAndSetHueInc(int hueIx, int hueA, int hueB, boolean shouldBePositive, int divisor);
	//void CopySatValFromColorRefToColorRef(COLORREF from, COLORREF &to);
	//void SetColorRef(double h, double s, double v, COLORREF& cr);
	//void SetAllColorRefs();

	///////////////////////////////////////////
	// Dialog
	void DisplayPalette();
	void InitColorControls();
	void EnableColorControls();
	//void SetColorControl(COLORREF clrRef, CMFCButton &clr, CMFCButton &clrSat);
	void InitSliders();
	//void InitColorControl(int hue, CMFCButton &clr, CMFCButton &clrSat, COLORREF &color);

	void InitColorControl(int colorNumber, int hue);
	
	void SavePaletteImage(CString filename);
	//void ClickColorButton(COLORREF &clrRef, CMFCButton &clr, CMFCButton &clrSat);
	void ClickColorButton(int colorNumber);
	boolean ColorsHaveChanged();
	void SortColorControls();
	void SetColorButton(int colorNumber);
	void SetColorButton(int colorNumber, COLORREF cr);

public:
	afx_msg void OnBnClickedSavecolortable();
	afx_msg void OnBnClickedClose();
	afx_msg void OnNMCustomdrawSliderHueinc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderSaturation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderBrightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton0();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedCheckHueOnly();
	afx_msg void OnBnClickedCheckLimitSliders();
	afx_msg void OnBnClickedColorOption();
	afx_msg void DrawPalette();
	afx_msg void OnTest();
};
