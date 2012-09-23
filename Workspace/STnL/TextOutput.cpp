// ********************************************************************
// Created:		2012/09/24
// Filename: 	TextOutput.cpp
// Author:		Rainsing
// Purpose:		在屏幕上输出文本
// ********************************************************************
#include "stdafx.h"
#include "TextOutput.h"

TextOutput::TextOutput( HWND hWnd )
{
	m_hWnd = hWnd;
	m_hDC = GetDC(m_hWnd);

	m_hFont = CreateFont(
		18, 8, 0, 0,
		FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_ROMAN,
		"Arial"
	);

	SelectObject(m_hDC, m_hFont);
}

TextOutput::~TextOutput()
{
	DeleteObject(m_hFont);
	ReleaseDC(m_hWnd, m_hDC);
}

void TextOutput::Print( const char* str, int x, int y )
{
	TextOut(m_hDC, x, y, str, strlen(str));
}
