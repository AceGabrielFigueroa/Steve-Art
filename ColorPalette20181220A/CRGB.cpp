#include "stdafx.h"
#include "CRGB.h"
#include "CHSV.h"

typedef DWORD COLORREF;


CRGB::CRGB()
{
	//R = 0;
	//G = 0;
	//B = 0;
}

CRGB::CRGB(unsigned char r, unsigned char g, unsigned char b)
{
	R = r;
	G = g;
	B = b;
}

CRGB::CRGB(COLORREF colorref)
{
	R = colorref & 0xFF;
	G = (colorref>>8) & 0xFF;
	B = (colorref>>16) & 0xFF;
}


CRGB::~CRGB()
{
}

bool CRGB::Equals(CRGB rgb)
{
	return (R == rgb.R) && (G == rgb.G) && (B == rgb.B);
}


COLORREF CRGB::GetColorRef()
{
	// 0x00bbggrr
	return (B << 16) + (G << 8) + R;
}

void CRGB::SetRGB(unsigned char r, unsigned char g, unsigned char b)
{
	R = r;
	G = g;
	B = b;
}

static double Min(double a, double b) {
	return a <= b ? a : b;
}

static double Max(double a, double b) {
	return a >= b ? a : b;
}

CHSV CRGB::RGBToHSV()
{
	double delta, min;
	double h = 0, s, v;

	min = Min(Min(R, G), B);
	v = Max(Max(R, G), B);
	delta = v - min;

	if (v == 0.0)
		s = 0;
	else
		s = delta / v;

	if (s == 0)
		h = 0.0;

	else
	{
		if (R == v)
		{
			h = (G - B) / delta;
		}
		else if (G == v)
		{ 
			h = 2 + (B - R) / delta;
		}
		else if (B == v)
		{
			h = 4 + (R - G) / delta;
		}

		h *= 60;

		if (h < 0.0)
		{
			h = h + 360;
		}
	}

	return CHSV(h, s, (v / 255));
}

