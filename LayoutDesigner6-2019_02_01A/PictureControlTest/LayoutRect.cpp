#include "stdafx.h"
#include "LayoutRect.h"


LayoutRect::LayoutRect()
{
	m_fromx = 0;
	m_fromy = 0;
	m_tox = 0;
	m_toy = 0;
}

LayoutRect::LayoutRect(int fromx, int fromy, int tox, int toy)
{
	m_fromx = fromx;
	m_fromy = fromy;
	m_tox = tox;
	m_toy = toy;
}

LayoutRect::~LayoutRect()
{
}


// CorrectXY finction makes sure that 
// From is upper left and 
// To is lower right

// Creates a new LayoutRect
LayoutRect LayoutRect::CorrectXY(LayoutRect r)
{
	return CorrectXY(r.m_fromx, r.m_fromy, r.m_tox, r.m_toy);
}

// Creates a new LayoutRect
LayoutRect LayoutRect::CorrectXY(int fromx, int fromy, int tox, int toy)
{
	// Correct Orientation of a LayoutRect is with 
	// fromx <= tox AND fromy <= toy
	int newFromX = fromx < tox ? fromx : tox;
	int newToX   = fromx < tox ? tox   : fromx;
	int newFromY = fromy < toy ? fromy : toy;
	int newToY   = fromy < toy ? toy   : fromy;
	return LayoutRect(newFromX, newFromY, newToX, newToY);
}

// Fixes LayoutRect in place
void LayoutRect::CorrectXY()
{
	// Correct Orientation of a LayoutRect is with 
	// fromx <= tox AND fromy <= toy
	int newFromX = m_fromx < m_tox ? m_fromx : m_tox;
	int newToX   = m_fromx < m_tox ? m_tox : m_fromx;
	int newFromY = m_fromy < m_toy ? m_fromy : m_toy;
	int newToY   = m_fromy < m_toy ? m_toy : m_fromy;
	m_fromx = newFromX;
	m_fromy = newFromY;
	m_tox = newToX;
	m_toy = newToY;
}

boolean LayoutRect::Equal(LayoutRect r)
{
	return	m_fromx == r.m_fromx && 
			m_fromy == r.m_fromy && 
			m_tox   == r.m_tox   && 
			m_toy   == r.m_toy;
}

boolean LayoutRect::operator==(const LayoutRect r)
{
	return	Equal(r);
}

LayoutRect LayoutRect::operator=(const LayoutRect r)
{
	m_fromx = r.m_fromx;
	m_fromy = r.m_fromy;
	m_tox   = r.m_tox;
	m_toy   = r.m_toy;
	return (LayoutRect)*this;
}

boolean LayoutRect::operator<(const LayoutRect &rhs)
{
	if (m_fromx < rhs.m_fromx)
		return true;

	return (m_fromx == rhs.m_fromx && m_fromy < rhs.m_fromy);
}
