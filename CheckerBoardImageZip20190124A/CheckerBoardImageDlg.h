// CheckerBoardImageDlg.h : header file
//

#pragma once
#include "CheckerBoard.h"
#include "ConfigParameter.h"

#include <ctime>
#include <ratio>
#include <chrono>
#include <vector>


using namespace std::chrono;


// CCheckerBoardImageDlg dialog
class CCheckerBoardImageDlg : public CDialogEx
{
// Construction
public:
	CCheckerBoardImageDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECKERBOARDIMAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
private:
	const int COLORS_SAFETY = 314159265;
	const int CHECKER_BOARD_SAFETY_VALUE = 873542197;
	const int CHECKER_BOARD_SAFETY_VALUE_2 = 791245378;
	int red[256], green[256], blue[256];

	// File Type: 1 - Single layer, 2 - Or2Layers, 3 Xor2Layers
	enum FILE_TYPE  { SINGLE_LAYER, TWO_OR, TWO_XOR };
	enum SHAPE { RECTANGLE, TRIANGLE, ELLIPSE, BROKEN_SQR, HOLE_IN_SQR, QUADRILATERAL, SHAPE_N };
	enum CONFIG_PARAMS { COLORTABLE };

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	int m_width;
	int m_height;
	int m_rows[8];
	int m_cols[8];
	int layerPairValues[8][2];
	CString colorTableFolder;
	CString imageFolder;
	boolean colorsRead = false;

	RGBQUAD* pRgbQuad;
	UINT8* pixelIndex;
	RGBQUAD pallette[256];

	BOOL m_random;	// When checked, each subclass of Boolean Bitmap will use 
					// a random number generator to determine how each 
					// bitmap is created.  If this is not checked, the
					// Refresh Random Seed control is disabled.

	BOOL m_refreshRandom;	// This is connected to the "Refresh Seed" check box
							// Usually it is checked and a new random number is 
							// generated each time an image is created.
							// The user would uncheck this if they wanted the exact same
							// bitmap with different colors.
	CButton m_refreshRandomCtrl;	// the "Refresh Seed" check box

	boolean configFileLoaded = false;
				// Set to false after a configuration is read from a file.
				// When the next image is created, this flag is checked and a 
				// new seed will NOT br created.  The seed read from the 
				// configuration fuile will be used instead.
	

	int m_numberOfImages;
	CheckerBoard *chBase[8];	// Basic CheckerBoards
	BooleanBitmap *ch[8];
	std::vector<int> heightDivisors;
	std::vector<int> widthDivisors;
	std::vector<ConfigParameter> configParameters;
	CString lastFileName;
	boolean nextImageClicked = false;
	UINT m_startTime;	// time stamp used for random seed
	int m_layerMode;
	int m_shape;

	CButton m_layerPairBox;
	CEdit m_layerPair[8][2];
	CComboBox m_comboRow[8];
	CComboBox m_comboCol[8];

	// I/O Related functions
	void SaveBMP(LPCTSTR Filename, clock_t start = -1);
	void TraceBMP(BITMAPFILEHEADER bmfh, BITMAPINFOHEADER bmih);
	CString GetOutputFileName();
	CString GetOutputFolderName();
	void GetNextLine(FILE* fp, LPTSTR buf, int buflen);
	boolean SaveConfiguration(CString fileName);
	boolean LoadConfiguration();
	boolean LoadConfiguration1(FILE *config);
	boolean LoadConfiguration2(FILE *config);
	CString BmpToTxt(CString from);
	CString TxtToBmp(CString from);
	char* CStringToCharStar(CString cs);
	void ReadColorTable(CString txtFileName);

	void CleanUp();
	void CreateBasicCheckerBoards();
	void CreatePixelIndex();
	void CreateOneLayerPixelIndex();
	void CreateTwoLayerPixelIndex(boolean xor=false);
	void RandomRowsCols();
	void TwoLayeredImages(boolean xor);
	void SingleLayer();
	void RandomLayerPairs();
	void ShowLayerPairs(BOOL show);
	void SingleLayerMultiple();
	void GetTimeString(LPTSTR buf, int buflen);
	void SetRandomSeed();
	void GetDivisors(int n, std::vector<int> *divisors);
	void ReadRowColDropLists();
	void SetRowColDropLists();
	void CreateRowColDropLists();
	boolean VerifyInput();
	void SaveConfigParam(int param, int value);
	void SaveConfigParam(int param, std::string value);
	void ProcessConfigParameters();
	double TriangleDistribution(double a, double b, double c);
	int TriDist(int a, int b);

	// Control Callback functions
	afx_msg void OnEnChangeWidth();
	afx_msg void OnEnChangeHeight();
	afx_msg void OnEnKillfocusWidth();
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedNextimage();
	afx_msg void OnBnClickedRandomPairs();
	afx_msg void OnBnClickedLayoutReset();
	afx_msg void OnBnClickedResetPairs();
	afx_msg void OnBnClickedGetColors();
	afx_msg void OnBnClickedCreateImage();
	afx_msg void OnBnClickedRandom();
	afx_msg void OnBnClickedMode();
	afx_msg void OnBnClickedLoadConfig();
	void OnOK();
	afx_msg void OnBnClickedCheckRandom();
};
