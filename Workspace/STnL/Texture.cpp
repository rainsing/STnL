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
	// wrapÑ°Ö··½Ê½
	if (u > 1.0f)
	{
		u -= 1.0f;
	}
	else if (u < 0.0f)
	{
		u += 1.0f;
	}

	if (v > 1.0f)
	{
		v -= 1.0f;
	}
	else if (v < 0.0f)
	{
		v += 1.0f;
	}

	Vector4 color;

	;

	return color;
}
