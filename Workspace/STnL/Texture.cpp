/********************************************************************
	created:	2012/09/21
	created:	21:9:2012   16:28
	filename: 	e:\GitHub\STnL\Workspace\STnL\Texture.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Texture
	file ext:	cpp
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "Texture.h"

#include "Utilities.h"

Texture::Texture( void )
{
	m_width = m_height = 0;
	m_colorData = NULL;
}

void Texture::Initialize( int width, int height )
{
	m_width = width;
	m_height = height;
	m_colorData = new unsigned char[m_width * m_height * 4];
}

Texture::~Texture()
{
	SAFE_VECTOR_DELETE(m_colorData);
}

Vector4 Texture::Sample( float u, float v )
{
	// Float2Int函数在浮点数十分接近一个整数时并不稳定。
	// 在这里做UV wrap时不可预估的输出将可能导致crash。
	// 为了安全起见选用慢一些的modf实现功能。

	/*if (u > 1.0f || u < 0.0f)
	{
		u -= Float2Int(u - 0.00001f);
	}

	if (v > 1.0f || v < 0.0f)
	{
		v -= Float2Int(v - 0.00001f);
	}*/

	double dummy;
	if (u > 1.0f)
	{
		u = (float)modf(double(u), &dummy);
	}
	else if (u < 0.0f)
	{
		u = 1.0f - (float)modf(double(u), &dummy);
	}

	if (v > 1.0f)
	{
		v = (float)modf(double(v), &dummy);
	}
	else if (v < 0.0f)
	{
		v = 1.0f - (float)modf(double(v), &dummy);
	}

	int texelIndex = (Float2Int(v * (m_height - 1.0f)) * m_width + Float2Int(u * (m_width - 1.0f))) << 2;

	Vector4 color;
	float r = 1.0f / 255.0f;
	color.x = m_colorData[texelIndex + 0] * r;
	color.y = m_colorData[texelIndex + 1] * r;
	color.z = m_colorData[texelIndex + 2] * r;
	color.w = m_colorData[texelIndex + 3] * r;

	return color;
}
