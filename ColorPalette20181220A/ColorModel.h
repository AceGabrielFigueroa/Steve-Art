#pragma once
#include "CRGB.h"
#include "CHSV.h"

class ColorModel
{
public:
	ColorModel();
	~ColorModel();
	void Init();

	enum COLOROPTION { ONE, TWO, THREE, FOUR, COMPLIMENTARY, TRIANGLE, SQUARE };
	const int MIN_SATURATION = 20;
	const int MIN_BRIGHTNESS = 65;
	const int MAXHUESPRREAD = 90;

	const int SAFETY_VALUE = 314159265;

private:
	/////////////////////////////////////////
	//
	//  These determine all aspects of a color tbale specification
	// These 3 have Range from 0 to 100
	int hueIncRange;
	int saturationRange;
	int valueRange;
	COLORREF clrs[4];
	int m_ColorOption;
	BOOL m_hue_only;
	BOOL m_limit_range;		// When true (ie checked) the max value of each of
							// the HueInc, Sat and Value sliders is restricted to what
							// "practical" values.  
							// Ie, ones that give subjectively pleasant palettes
							// User can overrid this by unchecking

	//////////////////////////////////////////
	// Used internal to ColorModel
	int m_hues[4];
	int m_hueInc[4][4];
	CRGB RGBpalette[256];

public:
	int hueFromColorRef(COLORREF colorRef);
	double saturationFromColorRef(COLORREF colorRef);
	double valueFromColorRef(COLORREF colorRef);
	boolean GeneratePalette();
	CRGB* GetPalette();
	void CalculateHuesAndIncrements();
	double CalculateHueSpread(int hueA, int hueB, boolean shoudBePositive);
	void SetHueInc(int hueIx, double hueInc);
	double CalculateAndSetHueInc(int hueIx);
	double CalculateAndSetHueInc(int hueIx, int hueA, int hueB, boolean shouldBePositive, int divisor);
	void CopySatValFromColorRefToColorRef(COLORREF from, COLORREF &to);
	void SetColorRefSatVal(int colorNumber, double s, double v);
	void SetAllColorRefs();
	void SortColors();
	void SaveColorTable(CString filename);

	///////////////////////////////////////////////////
	// Setters and getter
	void SetHueIncRange(int range);
	void SetSaturationRange(int range);
	void SetValueRange(int range);

	void SetColorRef(int i, COLORREF ref);
	COLORREF GetColorRef(int i);

	void SetLimitRange(boolean b);
	void SetHueOnly(boolean b);
	void SetColorOption( unsigned int option);
};

