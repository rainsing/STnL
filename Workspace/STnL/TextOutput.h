// ********************************************************************
// Created:		2012/09/24
// Filename: 	TextOutput.h
// Author:		Rainsing
// Purpose:		Utility class for printing text onto the client area of
//              the main window.
// ********************************************************************
#ifndef TextOutput_h__
#define TextOutput_h__

class TextOutput
{
public:
	TextOutput(HWND hWnd);
	~TextOutput();

	void Print(const char* str, int x, int y);

private:
	HWND m_hWnd;
	HDC m_hDC;
	HFONT m_hFont;
};

#endif // TextOutput_h__
