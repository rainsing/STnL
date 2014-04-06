/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   10:22
	filename: 	e:\GitHub\STnL\Workspace\STnL\BackBuffer.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	BackBuffer
	file ext:	cpp
	author:		Rainsing
	
	purpose:	All drawing operations will eventually write to this 'back buffer'. 
				At the end of each frame, Present() is called to blit the content of the buffer to the display.
				This is the only class that talks to the graphics hardware (through Windows GDI).
*********************************************************************/
#include "stdafx.h"
#include "BackBuffer.h"

BackBuffer::BackBuffer( HWND hWnd, int width, int height )
{
	m_hWnd = hWnd;
	m_width = width;
	m_height = height;

	m_windowDC = GetDC(m_hWnd);
	m_memoryDC = CreateCompatibleDC(m_windowDC);

	BITMAPINFO bitmapinfo;
	memset(&bitmapinfo, 0, sizeof(BITMAPINFO));

	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biWidth = m_width;
	bitmapinfo.bmiHeader.biHeight = -m_height;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;
	bitmapinfo.bmiHeader.biSizeImage = 0;
	bitmapinfo.bmiHeader.biClrUsed = 0;
	bitmapinfo.bmiHeader.biClrImportant = 0;

	m_hBitmap = CreateDIBSection(m_memoryDC, &bitmapinfo, DIB_RGB_COLORS, (void**)&m_colorData, NULL, 0);
	SelectObject(m_memoryDC, m_hBitmap);
}

BackBuffer::~BackBuffer()
{
	DeleteObject(m_hBitmap);
	DeleteDC(m_memoryDC);
	ReleaseDC(m_hWnd, m_windowDC);
}

void BackBuffer::Present( void )
{
	BitBlt(m_windowDC, 0, 0, m_width, m_height, m_memoryDC, 0, 0, SRCCOPY);
}

void BackBuffer::Clear( Color color )
{
	Color* cursor = m_colorData;
	Color* end = m_colorData + m_width * m_height;

	while (cursor < end)
	{
		*cursor++ = color;
	}
}

void BackBuffer::Clear( void )
{
	ZeroMemory(m_colorData, m_width * m_height * sizeof(Color));
}
