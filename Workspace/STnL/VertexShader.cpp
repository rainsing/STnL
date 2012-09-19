/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   15:42
	filename: 	e:\GitHub\STnL\Workspace\STnL\VertexShader.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	VertexShader
	file ext:	cpp
	author:		Rainisng
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "VertexShader.h"

#include "Vertex.h"

VertexShaderOutput::VertexShaderOutput( void )
{
	ZeroMemory(this, sizeof(VertexShaderOutput));
}

VertexShaderOutput MyVertexShader::Main( const Vertex& vertex )
{
	VertexShaderOutput output;

	output.clipPos = worldViewProjMatrix.Transform(vertex.position);

	return output;
}
