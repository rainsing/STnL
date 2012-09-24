/********************************************************************
	created:	2012/09/24
	created:	24:9:2012   10:19
	filename: 	e:\GitHub\STnL\Workspace\STnL\PixelShader.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	PixelShader
	file ext:	cpp
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "PixelShader.h"

#include "Texture.h"
#include "VertexShader.h"

Vector4 MyPixelShader::Main( VertexShaderOutput& vertexAttribute )
{
	return baseTexture->Sample(vertexAttribute.atrribute0.x, vertexAttribute.atrribute0.y);
}
