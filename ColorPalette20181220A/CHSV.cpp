#include "stdafx.h"
#include "CHSV.h"
#include "CRGB.h"
#include <cmath> 



CHSV::CHSV(double h, double s, double v)
{
	H = h;
	S = s;
	V = v;
}

bool CHSV::Equals(CHSV hsv)
{
	return (H == hsv.H) && (S == hsv.S) && (V == hsv.V);
}

CHSV::CHSV()
{
	//H = 0;
	//S = 0;
	//V = 0;
}

CHSV::~CHSV()
{
}

void CHSV::SetHSV(double h, double s, double v)
{
	H = h;
	S = s;
	V = v;
}


CRGB CHSV::xHSVToRGB(double H, double S, double V)
{
	double r = 0, g = 0, b = 0;

	if (S == 0)
	{
		r = V;
		g = V;
		b = V;
	}
	else
	{
		int i;
		double f, p, q, t;

		if (H == 360)
			H = 0;
		else
			H = H / 60;

		//i = (int)trunc(H);
		i = (int)floor(H);
		f = H - i;

		p = V * (1.0 - S);
		q = V * (1.0 - (S * f));
		t = V * (1.0 - (S * (1.0 - f)));

		switch (i)
		{
		case 0:
			r = V;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = V;
			b = p;
			break;

		case 2:
			r = p;
			g = V;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = V;
			break;

		case 4:
			r = t;
			g = p;
			b = V;
			break;

		default:
			r = V;
			g = p;
			b = q;
			break;
		}

	}

	return CRGB((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255));
}


CRGB CHSV::HSVToRGB(CHSV hsv)
{
	return xHSVToRGB(hsv.H, hsv.S, hsv.V);
}


CRGB CHSV::HSVToRGB()
{
	return xHSVToRGB(H, S, V);
}
