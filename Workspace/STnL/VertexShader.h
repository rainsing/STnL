/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   15:42
	filename: 	e:\GitHub\STnL\Workspace\STnL\VertexShader.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	VertexShader
	file ext:	h
	author:		Rainsing
	
	purpose:	Software vertex shader.
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
	Vector2 texCoord;
	Vector4 attribute0;		// custom attribute
};

VertexShaderOutput Lerp(VertexShaderOutput& va0, VertexShaderOutput& va1, float t);
void Lerp(VertexShaderOutput& out, VertexShaderOutput& va0, VertexShaderOutput& va1, float t);

class VertexShader
{
public:

	// The entry point of the vertex shader.
	// Any uniforms / constants can be defined as data member in derived classes.
	// This function is required to provide a 4-component clip space vertex position in its return value.
	virtual VertexShaderOutput Main(const Vertex& vertex) = 0;
};

class VsFixedFunction : public VertexShader
{
public:
	virtual VertexShaderOutput Main(const Vertex& vertex);

public:
	Matrix4 inverseWorldMatrix;
	Matrix4 worldViewProjMatrix;
	Vector3 lightPosition;
	Vector3 lightColor;
	Vector3 ambientColor;
};

class VsNormalMap : public VertexShader
{
public:
	virtual VertexShaderOutput Main(const Vertex& vertex);

public:
	Matrix4 worldMatrix;
	Matrix4 inverseWorldMatrix;
	Matrix4 worldViewProjMatrix;

	Vector3 lightPosition;
};

#endif // VertexShader_h__
