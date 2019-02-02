#pragma once
#ifndef _CSHV_
//#include "CHSV.h"
#endif
#define _CRBG_

//typedef DWORD COLORREF;

class CHSV;

class CRGB
{
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;

	CRGB();
	CRGB(unsigned char r, unsigned char g, unsigned char b);
	CRGB(COLORREF colorref);

	bool Equals(CRGB rgb);
	CHSV RGBToHSV();
	COLORREF GetColorRef();
	void SetRGB(unsigned char r, unsigned char g, unsigned char b);

	~CRGB();
};

