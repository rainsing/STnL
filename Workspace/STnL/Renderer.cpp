/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   17:30
	filename: 	e:\GitHub\STnL\Workspace\STnL\Renderer.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Renderer
	file ext:	cpp
	author:		Rainsing
	
	purpose:	实际负责执行渲染操作的对象
*********************************************************************/
#include "stdafx.h"
#include "Renderer.h"

#include "BackBuffer.h"
#include "Utilities.h"

Renderer::Renderer( void )
{
	m_renderTarget = NULL;
}

void Renderer::SetRenderTarget( BackBuffer* renderTarget )
{
	m_renderTarget = renderTarget;
}

void Renderer::DrawLine( int x0, int y0, int x1, int y1, Color color )
{
	bool steep = abs(y1 - y0) > abs(x1 - x0);

	if (steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int deltaX = x1 - x0;
	int deltaY = abs(y1 - y0);
	int error = deltaX / 2;
	int yStep = y0 < y1 ? 1 : -1;
	int y = y0;

	for (int x = x0; x <= x1; x++)
	{
		steep ? m_renderTarget->SetPixel(y, x, color) : m_renderTarget->SetPixel(x, y, color);
		error -= deltaY;
		if (error < 0)
		{
			y += yStep;
			error += deltaX;
		}
	}
}

void Renderer::Render( void )
{
	int x0 = 320, y0 = 240;
	int x1 = 300, y1 = 350;
	int x2 = 520, y2 = 400;

	DrawLine(x0, y0, x1, y1, COLOR_RGB(255, 255, 255));
	DrawLine(x0, y0, x2, y2, COLOR_RGB(255, 255, 255));
	DrawLine(x1, y1, x2, y2, COLOR_RGB(255, 255, 255));
}
