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

	output.position = worldViewProjMatrix.Transform(vertex.position);

	Vector4 worldSpaceNormal = worldMatrix.Transform(vertex.normal);

	Vector3 worldSpaceNormal3;
	worldSpaceNormal3.x = worldSpaceNormal.x;
	worldSpaceNormal3.y = worldSpaceNormal.y;
	worldSpaceNormal3.z = worldSpaceNormal.z;

	float angle = worldSpaceNormal3.Dot(Vector3(0.0f, 1.0f, 0.0f));
	if (angle < 0.0f)
	{
		angle = 0.0f;
	}
	output.atrribute0 = Vector3(0.0f, 1.0f, 0.0f) * angle;

	return output;
}

VertexShaderOutput Lerp( VertexShaderOutput& va0, VertexShaderOutput& va1, float t )
{
	VertexShaderOutput out;
	Lerp(out.position, va0.position, va1.position, t);
	Lerp(out.atrribute0, va0.atrribute0, va1.atrribute0, t);
	return out;
}

void Lerp( VertexShaderOutput& out, VertexShaderOutput& va0, VertexShaderOutput& va1, float t )
{
	 Lerp(out.position, va0.position, va1.position, t);
	 Lerp(out.atrribute0, va0.atrribute0, va1.atrribute0, t);
}
