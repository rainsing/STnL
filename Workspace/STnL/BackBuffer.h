/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   10:17
	filename: 	e:\GitHub\STnL\Workspace\STnL\BackBuffer.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	BackBuffer
	file ext:	h
	author:		Rainsing
	
	purpose:	�����Ⱦ�������л��Ʋ�����д�����BackBuffer����ÿһ֡
				����������Present()������buffer������blit����ʾ�豸�ϡ�
				������������Ⱦ����Ψһһ����Ӳ���򽻵��Ķ���
*********************************************************************/
#include "Types.h"

class BackBuffer
{
public:

	BackBuffer(HWND hWnd, int width, int height);
	~BackBuffer();

	void Clear(Color color);
	void Present(void);
	inline void SetPixel(int x, int y, Color color);

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
	*(m_colorData + y * m_width + x) = color;
}
