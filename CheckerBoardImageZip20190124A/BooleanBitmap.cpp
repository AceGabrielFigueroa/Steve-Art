#include "stdafx.h"
#include "BooleanBitmap.h"
#include <assert.h>


BooleanBitmap::BooleanBitmap()
{
}

BooleanBitmap::~BooleanBitmap()
{
	if (m_board != nullptr)
		delete[] m_board;
}

void BooleanBitmap::Construct(int width, int height)
{
	m_widthPixels = width;
	m_heightPixels = height;
	m_board = new boolean[m_widthPixels * m_heightPixels];
}

boolean BooleanBitmap::GetAt(int row, int col)
{
	assert(row >= 0 && row < m_heightPixels);
	assert(col >= 0 && col < m_widthPixels);
	return m_board[row * m_widthPixels + col];
}

void BooleanBitmap::SetAt(int row, int col, boolean value)
{
	assert(row >= 0 && row < m_heightPixels);
	assert(col >= 0 && col < m_widthPixels);

	m_board[row * m_widthPixels + col] = value;

	//char buffer[100];
	//sprintf_s(buffer, "SetAt Row %2d  Col %2d  %d addr  %0x\n", 
	//	row, col, value ? 1 : 0, &m_board[row * m_widthPixels + col]);
	//OutputDebugStringA(buffer);
}

BooleanBitmap::BooleanBitmap(BooleanBitmap* chb1, BooleanBitmap* chb2, boolean xor)
{
	// Combine the two checker boards with OR od XOR
	assert(chb1->m_widthPixels == chb2->m_widthPixels && chb1->m_heightPixels == chb2->m_heightPixels);

	Construct(chb1->m_widthPixels, chb1->m_heightPixels);
	for (auto row = 0; row < m_heightPixels; row++) {
		for (auto col = 0; col < m_widthPixels; col++)
		{
			boolean pixelValue = chb1->GetAt(row, col);
			if (xor)
				pixelValue ^= chb2->GetAt(row, col); // ^ is ExclusiveOr Op
			else
				pixelValue |= chb2->GetAt(row, col); // | is Or Op

			SetAt(row, col, pixelValue);
		}
	}
}

void BooleanBitmap::Copy(BooleanBitmap* fromBM, BooleanBitmap* toBM, 
	int fromCol, int fromRow, int toCol, int toRow, int wid, int hgt)
{
	ASSERT(fromCol + wid <= fromBM->m_widthPixels);
	ASSERT(toCol + wid <= toBM->m_widthPixels);
	ASSERT(fromRow + hgt <= fromBM->m_heightPixels);
	ASSERT(toRow + hgt <= toBM->m_heightPixels);
	for (auto col = 0; col < wid; col++)
	{
		int toCol_c = toCol + col;
		int fromCol_c = fromCol + col;
		for (auto row = 0; row < hgt; row++)
			//orig toBM->SetAt(toCol_c, toRow + row, fromBM->GetAt(fromCol_c, fromRow + row));
			toBM->SetAt(toRow + row, toCol_c, fromBM->GetAt(fromRow + row, fromCol_c));
	}
}

void BooleanBitmap::Copy(BooleanBitmap* fromBM, 
	int fromCol, int fromRow, int toCol, int toRow, int wid, int hgt)
{
	Copy(fromBM, this, fromCol, fromRow, toCol, toRow, wid, hgt);
}

void BooleanBitmap::RotateCounter(BooleanBitmap* fromBM, 
	int fromCol, int fromRow, int toCol, int toRow, int size)
{
	ASSERT(fromCol + size <= fromBM->m_widthPixels);
	ASSERT(toCol + size <= m_heightPixels);
	ASSERT(fromRow + size <= fromBM->m_heightPixels);
	ASSERT(toRow + size <= m_widthPixels);
	for (auto col = 0; col < size; col++)
	{
		int toCol_c = toCol + col;
		int fromCol_c = fromCol + col;
		for (auto row = 0; row < size; row++) 
		{
			//char buffer[200];
			int fromRow_r = fromRow + row;
			int toRow_r = toRow + row;
			boolean fromVal = fromBM->GetAt(fromRow_r, size - 1 - fromCol_c);
			//sprintf_s(buffer, "From (%d, %d)   To (%d, %d)  Value %c\n", 
			//	fromRow_r, size - 1 - fromCol_c, toCol_c, toRow_r, fromVal? '*' :'.');
			//OutputDebugStringA(buffer);
			//SetAt(toCol_c, toRow + row, fromBM->GetAt(fromRow + row, size - 1 - (fromRow + row)));
			SetAt(toRow_r, toCol_c, fromBM->GetAt(fromRow_r, size - 1 - fromCol_c));
		}
	}
}

//  Flip on Vertical Axis: Row is the same -- Col changes
//  AB   becomes  BA
//  CD            DC
void BooleanBitmap::FlipVert(BooleanBitmap* fromBM,	
	int fromCol, int fromRow, int toCol, int toRow, int wid, int hgt)
{
	ASSERT(fromCol + wid <= fromBM->m_widthPixels);
	ASSERT(toCol + wid <=m_widthPixels);
	ASSERT(fromRow + hgt <= fromBM->m_heightPixels);
	ASSERT(toRow + hgt <= m_heightPixels);
	for (auto col = 0; col < wid; col++)
	{
		int fromCol_c = fromCol + col;
		int toCol_c = toCol + (wid-1-col);
		for (auto row = 0; row < hgt; row++)
		{
			//boolean v = fromBM->GetAt(wid - 1 - fromCol_c, fromRow + row);
			//SetAt(toCol_c, toRow + row, v );
			//boolean v = fromBM->GetAt(fromCol_c, fromRow + row);
			//SetAt(toCol_c, toRow + row, v);
			boolean v = fromBM->GetAt(fromRow + row , fromCol_c);
			SetAt(toRow + row, toCol_c, v);
		}
	}
}

//  Flip on Horizontal Axis: Col is the same -- Row changes
//  AB   becomes  CD
//  CD            AB
void BooleanBitmap::FlipHoriz(BooleanBitmap* fromBM,
	int fromCol, int fromRow, int toCol, int toRow, int wid, int hgt)
{
	ASSERT(fromCol + wid <= fromBM->m_widthPixels);
	ASSERT(toCol + wid <= m_widthPixels);
	ASSERT(fromRow + hgt <= fromBM->m_heightPixels);
	ASSERT(toRow + hgt <= m_heightPixels);
	for (auto col = 0; col < wid; col++)
	{
		int fromCol_c = fromCol + col;
		int toCol_c = toCol + col;
		for (auto row = 0; row < hgt; row++)
		{
			//boolean v = fromBM->GetAt(fromCol_c, hgt - 1 - fromRow - row);
			//SetAt(toCol_c, toRow + row, v );
			boolean v = fromBM->GetAt( fromRow + row, fromCol_c);
			SetAt(toRow + (hgt-1-row), toCol_c, v);
		}
	}
}

//void CheckerBoard::ConvertToInt()
//{
//	UINT32 pixmask[32] = {
//		0x00000001, 0x00000002, 0x00000004, 0x00000008,
//		0x00000010, 0x00000020, 0x00000040, 0x00000080,
//		0x00000100, 0x00000200, 0x00000400, 0x00000800,
//		0x00001000, 0x00002000, 0x00004000, 0x00008000, 
//		0x00010000, 0x00020000, 0x00040000, 0x00080000,
//		0x00100000, 0x00200000, 0x00400000, 0x00800000,
//		0x01000000, 0x02000000, 0x04000000, 0x08000000,
//		0x10000000, 0x20000000, 0x40000000, 0x80000000
//	};
//
//	for (auto row = 0; row < m_rows; row++) {
//		for (auto col = 0; col < m_columns; col += sizeof(UINT32)) {
//			UINT32 pixelChunk = 0;
//			for (auto i = 0; i < sizeof(UINT32); i++) {
//				if (m_board[row][col]) 
//				{
//					pixelChunk += pixmask[i];
//				}
//			}
//			m_pixels[row][col / 32] = pixelChunk;
//		}
//	}
//}

void BooleanBitmap::Show()
{
	char bufr[200];
#define Ou OutputDebugStringA
#define CH(c) sprintf_s(bufr, "%c", c);
#define NL() CH('\n');

	CH('-');
	sprintf_s(bufr, "\nRows %d  Cols %2d\n", m_heightPixels, m_widthPixels);
	for (auto row = 0; row < m_heightPixels; row++) {
		for (auto col = 0; col < m_widthPixels; col++) {
			CH(GetAt(row, col) ? '*' : '.'); Ou(bufr);
		}
		NL(); Ou(bufr);
	}
	CH('-');
	NL();
}

// returns real (0.0, n) ie open interval.
// 
double  BooleanBitmap::drand(int n)
{
	return (double)(rand() % n) / (double)n;
}

// returns int 0 to n-1
int BooleanBitmap::irand(int n)
{
	return rand() % n;
}

// returns int lo to hi inlusive both ends [lo, hi]
int BooleanBitmap::irand(int lo, int hi)
{
	return lo + rand() % (hi-lo+1);
}