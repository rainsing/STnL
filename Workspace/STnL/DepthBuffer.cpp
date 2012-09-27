/********************************************************************
	created:	2012/09/24
	created:	24:9:2012   18:13
	filename: 	e:\GitHub\STnL\Workspace\STnL\DepthBuffer.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	DepthBuffer
	file ext:	cpp
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "DepthBuffer.h"

#include "Utilities.h"

DepthBuffer::DepthBuffer( int width, int height )
{
	m_width = width;
	m_height = height;
	m_depthData = new float[width * height];
}

DepthBuffer::~DepthBuffer()
{
	SAFE_VECTOR_DELETE(m_depthData);
}

void DepthBuffer::Clear( void )
{
	float* cursor = m_depthData;
	float* end = m_depthData + m_width * m_height;

	while (cursor < end)
	{
		*cursor++ = 1.0f;
	}
}

bool DepthBuffer::TestDepth( int x, int y, float depth )
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
	{
		return false;
	}

	float& depthInBuffer = m_depthData[y * m_width + x];
	bool pass = depth < depthInBuffer;

	// 通过了测试就写深度
	if (pass)
	{
		depthInBuffer = depth;
	}

	return pass;
}
