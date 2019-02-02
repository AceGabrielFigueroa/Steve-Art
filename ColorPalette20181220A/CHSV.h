#pragma once
//#ifndef _CRBG_
//#include "CRGB.h"
//#endif
//#define _CSHV_

class CRGB;

class CHSV
{
public:
	double H;
	double S;
	double V;

	CHSV();
	CHSV(double h, double s, double v);

	bool Equals(CHSV hsv);
	CRGB HSVToRGB(CHSV hsv);
	CRGB HSVToRGB();
	void SetHSV(double h, double s, double v);
	static CRGB xHSVToRGB(double H, double S, double V);

	~CHSV();
};

