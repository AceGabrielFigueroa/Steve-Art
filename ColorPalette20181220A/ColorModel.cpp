#include "stdafx.h"
#include "ColorModel.h"


ColorModel::ColorModel()
{
}


ColorModel::~ColorModel()
{
}

void ColorModel::Init()
{

}

int ColorModel::hueFromColorRef(COLORREF colorRef)
{
	CRGB c(colorRef);
	CHSV hsv = c.RGBToHSV();

	return (int)hsv.H;
}

double ColorModel::saturationFromColorRef(COLORREF colorRef)
{
	CRGB c(colorRef);
	CHSV hsv = c.RGBToHSV();

	return hsv.S;
}

double ColorModel::valueFromColorRef(COLORREF colorRef)
{
	CRGB c(colorRef);
	CHSV hsv = c.RGBToHSV();

	return hsv.V;
}

// We sort the colors so that when we are generating the hue increments
// we compare each color with the next one in sequence on the color wheel
// This should be simple.
// However, for some of the Color Options (enum COLOROPTION), we don't use all
// of the colors (some are implied anc computed).
// So we only want to sort the ones we are actually using
// 
// These are the one the user actually specifies:
// (1) ONE, COMPLIMENTARY, TRIANGLE, SQUARE  -- 0
// (2) TWO                                   -- 0, 2
// (3) THREE                                 -- 0, 2, 3
// (4) FOUR                                  -- 0, 1, 2 ,3
// These may not be obvious.
// (1) User specifies 1 color and we fill in the other three
//     based on the hueIncRange
// (2) User specifies 2 colors.  We pair 0 & 1 and 2 & 3 to extend the hue increment
// (3) User specifies 2 colors.  
//     We have an extra color, so we pair 0 & 1 to extend the hue increment
// (4) Hopefully this is obvious
void ColorModel::SortColors()
{
	int colorsToSort[4] = { 0,1,2,3 };
	int numToSort = 4;
	switch (m_ColorOption)
	{
	case ONE:
	case COMPLIMENTARY:
	case TRIANGLE:
	case SQUARE:
		// Nothing to sort
		numToSort = 0;
		break;
	case TWO:
		colorsToSort[0] = 0;
		colorsToSort[1] = 2;
		numToSort = 2;
		break;
	case THREE:
		colorsToSort[0] = 0;
		colorsToSort[1] = 2;
		colorsToSort[2] = 3;
		numToSort = 3;
		break;
	case FOUR:
		// use default initialization
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	for (auto i = 0; i < 4; i++)
		m_hues[i] = hueFromColorRef(clrs[i]);

	for (auto i = 0; i < numToSort-1; i++)
		for (auto j = i + 1; j < numToSort; j++)
			if (m_hues[colorsToSort[j]] < m_hues[colorsToSort[i]])
			{
				int hue = m_hues[colorsToSort[j]];
				m_hues[colorsToSort[j]] = m_hues[colorsToSort[i]];
				m_hues[colorsToSort[i]] = hue;

				COLORREF ref = clrs[colorsToSort[j]];
				clrs[colorsToSort[j]] = clrs[colorsToSort[i]];
				clrs[colorsToSort[i]] = ref;
			}


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
}

COLORREF ColorModel::GetColorRef(int i)
{
	return clrs[i];
}

boolean ColorModel::GeneratePalette()
{
	CalculateHuesAndIncrements();

	////////////////////////////////
	// Read slider controls
	//int saturation = m_saturationSlider.GetPos();	// range 0 to 100
	//int brightness = m_brightnessSlider.GetPos();	// range 0 to 100

	// The value from the sliders are from 0 to 100, think of it as a percent

	// For saturation and brightness, the user can set a checkbox to indicate 
	// whether use the full range or a restricted range that avoids  
	// too many pixels that are almost white or black
	int minsat = m_limit_range ? MIN_SATURATION : 0;
	int minbrt = m_limit_range ? MIN_BRIGHTNESS : 0;

	int saturationIncrement = (int)((saturationRange / 100.0)*((100 - minsat) / 3.0));
	int brightnessIncrement = (int)((valueRange / 100.0)*((100 - minbrt) / 3.0));

	char str[256];
	sprintf_s(str, "CM: GeneratePalette -- Sat-Inc %d Bri-Inc %d\n", saturationIncrement, brightnessIncrement);
	OutputDebugStringA(str);

	int a_brightness[4];
	int a_saturation[4];
	for (int i = 0; i < 4; i++)
	{
		a_saturation[i] = 100 - (i * saturationIncrement);
		a_brightness[i] = 100 - (i * brightnessIncrement);
	}


	//*******************************************************
	// Now compute the full palette of 256 colors
	for (int hueIx = 0; hueIx < 4; hueIx++)
	{
		double satToUse = m_hue_only ? 1.0 : saturationFromColorRef(clrs[hueIx]);
		double valueToUse = m_hue_only ? 1.0 : valueFromColorRef(clrs[hueIx]);

		sprintf_s(str, "CM: GeneratePalette -- color %d hue %3d -- HueInc", hueIx, m_hues[hueIx]);
		OutputDebugStringA(str);

		float hue = 0.0;
		float sat = 0.0;

		for (int hueIncIx = 0; hueIncIx < 4; hueIncIx++)
		{
			hue = (float)(m_hues[hueIx] + m_hueInc[hueIx][hueIncIx]);
			sprintf_s(str, " %2d", m_hueInc[hueIx][hueIncIx]);
			OutputDebugStringA(str);
			if (hueIncIx == 3)
			{
				OutputDebugStringA("\n");
			}

			for (int satIx = 0; satIx < 4; satIx++)
			{
				sat = satToUse * (float)(a_saturation[satIx] / 100.0);
				for (int brtIx = 0; brtIx < 4; brtIx++)
				{
					int index = 64 * hueIx + 16 * hueIncIx + 4 * satIx + brtIx;
					float val = valueToUse * (float)(a_brightness[brtIx] / 100.0);
					//char str[256];
					//sprintf_s(str, "index %d  hueIx %d hueInc %d  sat %d  brt %d - H %3.0f  S %3.3f  V %3.3f\n", 
					//	index, hueIx, hueIncIx, satIx, brtIx, hue, sat, val);
					//OutputDebugStringA(str);

					// Create RGB Palette
					CHSV hsv = CHSV(hue, sat, val);
					RGBpalette[index] = hsv.HSVToRGB();
				}
			}
		}
	}
	return true;
}


void ColorModel::SetAllColorRefs()
{
	double sat = saturationFromColorRef(clrs[0]);
	double val = valueFromColorRef(clrs[0]);
	SetColorRefSatVal( 0, sat, val);
	SetColorRefSatVal( 1, sat, val);
	SetColorRefSatVal( 2, sat, val);
	SetColorRefSatVal( 3, sat, val);
}

void ColorModel::CalculateHuesAndIncrements()
{
	//UpdateData(TRUE);	// Read Color Option radio button

	////////////////////////////////////////////////////////////////////////////
	// When we calculate the hue increments, for each base hue,
	// we need to look at the next hue around the color wheel.
	// Then we take the difference between them and base 
	// and base the increments on that difference

	// Sort the hues and find maxHue Inc for each hue 
	// at 1/2 the difference between adjacent hues.

	// The hue increment values will be different for each hue.
	// if the limit Slider boxed is checked, we will use one half 
	// of the difference between the hues., with a max of 1/4 of the way around the wheel

	// We need to limit maxHueInc in some cases when there is a very large gap
	// between hues.  90 is a guess for a workable value

	//////////////////////////////////////////////////////////////////////////////
	// For the cases TWO, THREE and FOUR, the hues may be close together or spread far apart
	// In general, we determine the hue increments from the base hue by 
	// letting them extend half way to the next hue 
	// (when limitSliders is unchecked, we go all the way to the next hue)
	// The possible problem with this is that for some hues, the increments
	// may be small and for others they are large.
	// It is possible that this effect may be desired, in which case uncheck "limitSliders"
	// But with "limitSliders" unchecked, the hues can cover most of the color wheel
	// interfering with a possbile desire to limit the color pallette.

	//int maxinc = m_limit_range ? MAX_HUE_INCREMENT : 100;
	//double hueIncSlider = ((double)m_hueIncSlider.GetPos()) / 100.0; // 0 to 1
	m_hues[0] = hueFromColorRef(clrs[0]);
	m_hues[1] = hueFromColorRef(clrs[1]);
	m_hues[2] = hueFromColorRef(clrs[2]);
	m_hues[3] = hueFromColorRef(clrs[3]);

	double hueSpread = 0.0;
	char str[256];
	sprintf_s(str, "CM: Calc-Hues-Inc -- Hues %d %d %d %d\n", m_hues[0], m_hues[1], m_hues[2], m_hues[3]);
	OutputDebugStringA(str);

	switch (m_ColorOption)
	{
	case ONE:
	{
		// With one color - if limitSliders is true, the hue range 
		// will be one half (1/2) of the color wheel
		hueSpread = (hueIncRange/100.0) * (m_limit_range ? 180.0 : 360.0);
		m_hues[0] = hueFromColorRef(clrs[0]);
		m_hues[1] = m_hues[0] + hueSpread * 0.25;
		m_hues[2] = m_hues[0] + hueSpread * 0.50;
		m_hues[3] = m_hues[0] + hueSpread * 0.75;
		SetAllColorRefs();

		for (auto i = 0; i < 4; i++)
			SetHueInc(i, hueSpread / 16.0);
	}
	break;
	case COMPLIMENTARY:
	{
		// With complimentary colors - if limitSliders is true, the hue range 
		// will be one quarter (1/4) of the color wheel
		hueSpread = (hueIncRange / 100.0) * (m_limit_range ? 90.0 : 180.0);
		m_hues[0] = hueFromColorRef(clrs[0]);
		m_hues[1] = (m_hues[0] + (int)(hueSpread * 0.5)) % 360;
		m_hues[2] = (m_hues[0] + 180) % 360;
		m_hues[3] = (m_hues[2] + (int)(hueSpread * 0.5)) % 360;
		SetAllColorRefs();

		for (auto i = 0; i < 4; i++)
			SetHueInc(i, hueSpread / 8.0);
	}
	break;

	case TRIANGLE:
	{
		// With color triangle - if limitSliders is true, the hue range 
		// will be one sixth (1/6) of the color wheel.
		// The color designated by the single color that defines this config, 
		// will have two of the primary hues;
		hueSpread = (hueIncRange / 100.0) * (m_limit_range ? 60.0 : 120.0);
		m_hues[0] = hueFromColorRef(clrs[0]);
		m_hues[1] = (m_hues[0] + (int)(hueSpread * 0.5)) % 360;
		m_hues[2] = (m_hues[0] + 120) % 360;
		m_hues[3] = (m_hues[0] + 240) % 360;
		SetAllColorRefs();

		SetHueInc(0, hueSpread / 8.0);
		SetHueInc(1, hueSpread / 8.0);
		SetHueInc(2, hueSpread / 4.0);
		SetHueInc(3, hueSpread / 4.0);
	}
	break;

	case SQUARE:
	{
		// With color square - if limitSliders is true, the hue range 
		// will be one eighth (1/8) of the color wheel.
		hueSpread = (hueIncRange / 100.0) * (m_limit_range ? 45.0 : 90.0);
		m_hues[0] = hueFromColorRef(clrs[0]);
		m_hues[1] = (m_hues[0] + 90) % 360;
		m_hues[2] = (m_hues[1] + 90) % 360;
		m_hues[3] = (m_hues[2] + 90) % 360;
		SetAllColorRefs();

		for (auto i = 0; i < 4; i++)
			SetHueInc(i, hueSpread / 4.0);
	}
	break;

	case TWO:
	{
		// Need to Sort Color Controls
		//  clrs[1] and clrs[3] are not set in the UI
		// Set clrs[1] to clrs[0] and Set clrs[3] to clrs[2]
		clrs[1] = clrs[0];
		clrs[3] = clrs[2];
		//SortColorControls();
		// AA SortColors();

		//hueSpread = CalculateHueSpread(m_hues[0], m_hues[2], true);
		//SetHueInc(0, hueSpread / 8.0);
		hueSpread = CalculateAndSetHueInc(0, m_hues[0], m_hues[2], true, 8);
		m_hues[1] = m_hues[0] + hueSpread / 2;
		SetHueInc(1, hueSpread / 8.0);

		//hueSpread = CalculateHueSpread(m_hues[2], m_hues[0], false);
		//SetHueInc(2, hueSpread / 8.0);
		hueSpread = CalculateAndSetHueInc(2, m_hues[2], m_hues[0], false, 8);
		m_hues[3] = m_hues[2] + hueSpread / 2;
		SetHueInc(3, hueSpread / 8.0);
	}
	break;

	case THREE:
	{
		// Need to Sort Color Controls
		// Set clrs[1] to clrs[0] because clrs[1] is not set in the UI
		// clrs[1] is set in this piece of code
		clrs[1] = clrs[0];
		//SortColorControls();
		// AA SortColors();

		m_hues[0] = hueFromColorRef(clrs[0]);
		m_hues[2] = hueFromColorRef(clrs[2]);
		m_hues[3] = hueFromColorRef(clrs[3]);

		hueSpread = CalculateAndSetHueInc(0, m_hues[0], m_hues[2], true, 8);
		m_hues[1] = m_hues[0] + hueSpread / 2;
		SetHueInc(1, hueSpread / 8.0);

		CalculateAndSetHueInc(2, m_hues[2], m_hues[3], true, 4);
		CalculateAndSetHueInc(3, m_hues[3], m_hues[0], false, 4);
	}
	break;

	case FOUR:
	{
		//SortColorControls();
		// AA SortColors();

		//m_hues[0] = hueFromColorRef(clrs[0]);
		//m_hues[1] = hueFromColorRef(clrs[1]);
		//m_hues[2] = hueFromColorRef(clrs[2]);
		//m_hues[3] = hueFromColorRef(clrs[3]);

		//CalculateAndSetHueInc(0, m_hues[0], m_hues[1], true, 4);
		//CalculateAndSetHueInc(1, m_hues[1], m_hues[2], true, 4);
		//CalculateAndSetHueInc(2, m_hues[2], m_hues[3], true, 4);
		//CalculateAndSetHueInc(3, m_hues[3], m_hues[0], false, 4);
		CalculateAndSetHueInc(0);
		CalculateAndSetHueInc(1);
		CalculateAndSetHueInc(2);
		CalculateAndSetHueInc(3);
	}
	break;

	default:
		ASSERT(FALSE);
		break;
	}
}

void ColorModel::CopySatValFromColorRefToColorRef(COLORREF from, COLORREF &to)
{
	CRGB fromRGB(from);
	CHSV fromHSV = fromRGB.RGBToHSV();
	double sat = fromHSV.S;
	double val = fromHSV.V;

	CRGB toRGB(to);
	CHSV toHSV = toRGB.RGBToHSV();
	double hue = toHSV.H;
	toHSV.V = val;
	toHSV.S = sat;

	toRGB = toHSV.HSVToRGB();
	to = toRGB.GetColorRef();
}

double ColorModel::CalculateHueSpread(int hueA, int hueB, boolean shouldBePositive)
{
	//UpdateData(TRUE);	// Read Color Option radio button
	//double hueIncSlider = ((double)m_hueIncSlider.GetPos()) / 100.0; // 0 to 1
	//double hueIncSlider = hueIncRange;
	int diff = hueB - hueA;
	//if (shouldBePositive)
	//	ASSERT(diff >= 0);
	//else
	//	ASSERT(diff <= 0);
	if (diff < 0)
		diff += 360;

	double fullHueSpread = (hueIncRange/100.0) * diff * (m_limit_range ? 0.5 : 1.0);
	// We need to limit hueSpread in some cases when there is a very large gap
	// between hues.  90 is a guess for a workable value
	double hueSpread = (m_limit_range && fullHueSpread > MAXHUESPRREAD) ? MAXHUESPRREAD : fullHueSpread;
	return hueSpread;
}

void ColorModel::SetHueInc(int hueIx, double hueInc)
{
	for (auto i = 0; i < 4; i++)
	{
		m_hueInc[hueIx][i] = hueInc * i;
	}
}

double ColorModel::CalculateAndSetHueInc(int hueIx, int hueA, int hueB, boolean shouldBePositive, int divisor)
{
	double hueSpread = CalculateHueSpread(hueA, hueB, shouldBePositive);
	SetHueInc(hueIx, hueSpread / (double)divisor);
	return hueSpread;
}

double ColorModel::CalculateAndSetHueInc(int hueIx)
{
	// Find next hue by finding minimum difference with other hues
	char str[200];

	int huediff;
	int mindiff = 360;
	for (auto i = 0; i < 4; i++)
	{
		huediff = m_hues[i] - m_hues[hueIx];
		//sprintf_s(str, "CM: CalculateAndSetHueInc -- HueIx (%d %3d)  i (%d %d)  dif %d\n",
		//	hueIx, m_hues[hueIx], i, m_hues[i], huediff);
		//OutputDebugStringA(str);

		if (huediff <= 0)
			huediff += 360;
		//sprintf_s(str, "CM: CalculateAndSetHueInc --  dif %d  mindiff %d\n",
		//	huediff, mindiff);
		//OutputDebugStringA(str);

		if (huediff < mindiff)
			mindiff = huediff;
		//sprintf_s(str, "CM: CalculateAndSetHueInc --  dif %d  mindiff %d\n",
		//	huediff, mindiff);
		//OutputDebugStringA(str);
	}

	double fullHueSpread = (hueIncRange / 100.0) * mindiff * (m_limit_range ? 0.5 : 1.0);
	// We need to limit hueSpread in some cases when there is a very large gap
	// between hues.  90 is a guess for a workable value
	double hueSpread = (m_limit_range && fullHueSpread > MAXHUESPRREAD) ? MAXHUESPRREAD : fullHueSpread;

	SetHueInc(hueIx, hueSpread / 4.0);
	return 0.0;
}

void ColorModel::SetHueIncRange(int range)
{
	hueIncRange = range;
}

void ColorModel::SetSaturationRange(int range)
{
	saturationRange = range;
}

void ColorModel::SetValueRange(int range)
{
	valueRange = range;
}

void ColorModel::SetColorRef(int i, COLORREF ref)
{
	clrs[i] = ref;
	//char str[200];
	//sprintf_s(str, "CM: SetColorRef --     color %d  Ref %6x, R %3d G %3d B %3d\n",
	//	i, ref, GetRValue(ref), GetGValue(ref), GetBValue(ref));
	//OutputDebugStringA(str);
}


void ColorModel::SetColorRefSatVal(int colorNumber, double s, double v)
{
	CRGB rgb = CHSV::xHSVToRGB(m_hues[colorNumber], s, v);
	clrs[colorNumber] = rgb.GetColorRef();

	//char str[200];
	//sprintf_s(str, "CM: SetColorRefSV -- color %d  COLORREF %6x, R %3d G %3d B %3d  Hue %3d\n",
	//	colorNumber, clrs[colorNumber], rgb.R, rgb.B, rgb.B, (int)m_hues[colorNumber]);
	//OutputDebugStringA(str);
}

void ColorModel::SetLimitRange(boolean b)
{
	m_limit_range = b;
}

void ColorModel::SetHueOnly(boolean b)
{
	m_hue_only = b;
}

void ColorModel::SetColorOption(unsigned int option)
{
	m_ColorOption = option;
}

CRGB* ColorModel::GetPalette()
{
	return &RGBpalette[0];
}


void ColorModel::SaveColorTable(CString fileName)
{
	CStdioFile file;
	UINT flags = CFile::modeCreate | CFile::modeWrite;// | CFile::typeText;

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
		AfxMessageBox(err);
		return;
	}

	//// Ready to write
	char str[200];

	GeneratePalette();

	// Safety Value - check this when we read it to avoid reading an inappropriate file
	sprintf_s(str, "%d\n\0", SAFETY_VALUE);
	file.Write((LPCTSTR)str, strlen(str));

	// SAve Parameters used to create color tabnle as comments 
	//
	// Write 4 Hues
	//sprintf_s(str, "# Hues %d %d %d %d\n#\n\0", m_hues[0], m_hues[1], m_hues[2], m_hues[3]);
	//file.Write((LPCTSTR)str, strlen(str));
	//
	// Write 4 Hue Increments
	//sprintf_s(str, "# HueInc %d %d %d %d\n#\n\0", m_hueInc[0], m_hueInc[1], m_hueInc[2], m_hueInc[3]);
	//file.Write((LPCTSTR)str, strlen(str));
	//
	// Write 4 Saturation
	//sprintf_s(str, "# Saturation %d %d %d %d\n#\n\0", m_saturation[0], m_saturation[1], m_saturation[2], m_saturation[3]);
	//file.Write((LPCTSTR)str, strlen(str));
	//
	// Write 4 Brightness
	//sprintf_s(str, "# Brightness %d %d %d %d\n#\n\0", m_brightness[0], m_brightness[1], m_brightness[2], m_brightness[3]);
	//file.Write((LPCTSTR)str, strlen(str));
	//
	// Now write out color Data
	//CRGB* RGBpalette = GetPalette();

	strcpy_s(str, "# Colors  Index  Red  Green Blue\n\0");
	file.Write((LPCTSTR)str, strlen(str));

	for (int i = 0; i < 256; i++)
	{
		CRGB rgb = RGBpalette[i];

		sprintf_s(str, "%d %d %d %d\n\0", i, rgb.R, rgb.G, rgb.B);
		file.Write((LPTSTR)str, strlen(str));
	}

	file.Close();
}