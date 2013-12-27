/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   10:22
	filename: 	e:\GitHub\STnL\Workspace\STnL\BackBuffer.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	BackBuffer
	file ext:	cpp
	author:		Rainsing
	
	purpose:	软件渲染器的所有绘制操作都写入这个BackBuffer，在每一帧
				的最后调用其Present()函数将buffer的内容blit到显示设备上。
				这是这个软件渲染器中唯一一个和硬件打交道的对象。本例使用
				Windows GDI来实现blitting。
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
	bitmapinfo.bmiHeader.biHeight = -m_height;	// 自顶向下的DIB，原点在左上角
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
