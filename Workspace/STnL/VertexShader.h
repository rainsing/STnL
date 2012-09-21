/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   15:42
	filename: 	e:\GitHub\STnL\Workspace\STnL\VertexShader.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	VertexShader
	file ext:	h
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#ifndef VertexShader_h__
#define VertexShader_h__

#include "Vector.h"
#include "Matrix.h"

class Vertex;

class VertexShaderOutput
{
public:
	VertexShaderOutput(void);

public:
	Vector4 position;
	Vector4 atrribute0;
};

VertexShaderOutput Lerp(VertexShaderOutput& va0, VertexShaderOutput& va1, float t);
void Lerp(VertexShaderOutput& out, VertexShaderOutput& va0, VertexShaderOutput& va1, float t);

class VertexShader
{
public:
	virtual VertexShaderOutput Main(const Vertex& vertex) = 0;
};

class MyVertexShader : public VertexShader
{
public:
	virtual VertexShaderOutput Main(const Vertex& vertex);

public:
	Matrix4 worldMatrix;
	Matrix4 worldViewProjMatrix;
};

#endif // VertexShader_h__
