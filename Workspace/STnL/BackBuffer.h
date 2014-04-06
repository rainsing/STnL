/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   10:17
	filename: 	e:\GitHub\STnL\Workspace\STnL\BackBuffer.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	BackBuffer
	file ext:	h
	author:		Rainsing
	
	purpose:	All drawing operations will eventually write to this 'back buffer'. 
				At the end of each frame, Present() is called to blit the content of the buffer to the display.
				This is the only class that talks to the graphics hardware (through Windows GDI).
*********************************************************************/
#ifndef BackBuffer_h__
#define BackBuffer_h__

#include "Types.h"

class BackBuffer
{
public:

	BackBuffer(HWND hWnd, int width, int height);
	~BackBuffer();

	void Clear(Color color);
	void Clear(void);	// fast clear to pure black
	void Present(void);
	inline void SetPixel(int x, int y, Color color);
	inline int GetWidth(void);
	inline int GetHeight(void);

private:

	HWND m_hWnd;
	int m_width;
	int m_height;
	Color* m_colorData;

	HDC m_windowDC;
	HDC m_memoryDC;
	HBITMAP m_hBitmap;
};

void BackBuffer::SetPixel( int x, int y, Color color )
{
	// TODO: This won't be needed any more after clipping is implemented.
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
	{
		return;
	}

	*(m_colorData + y * m_width + x) = color;
}

int BackBuffer::GetWidth( void )
{
	return m_width;
}

int BackBuffer::GetHeight( void )
{
	return m_height;
}

#endif // BackBuffer_h__
