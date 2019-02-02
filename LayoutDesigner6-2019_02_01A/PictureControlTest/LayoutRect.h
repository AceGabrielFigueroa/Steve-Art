#pragma once
class LayoutRect
{
public:
	int m_fromx, m_fromy, m_tox, m_toy;
	LayoutRect();

	LayoutRect(int fromx, int fromy, int tox, int toy);
	~LayoutRect();
	void CorrectXY();
	static LayoutRect CorrectXY(LayoutRect r);
	static LayoutRect CorrectXY(int fromx, int fromy, int tox, int toy);
	boolean Equal(LayoutRect r);
	LayoutRect operator=(const LayoutRect r);
	boolean operator==(const LayoutRect r);

	boolean operator<(const LayoutRect &rhs);

};

