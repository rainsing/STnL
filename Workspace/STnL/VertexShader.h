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
	Vector2 texCoord;
	Vector4 attribute0;
};

VertexShaderOutput Lerp(VertexShaderOutput& va0, VertexShaderOutput& va1, float t);
void Lerp(VertexShaderOutput& out, VertexShaderOutput& va0, VertexShaderOutput& va1, float t);

class VertexShader
{
public:
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
	Vector3 diffuseColor;
	Vector3 ambientColor;
};

class VsFixedFunctionAltUv : public VertexShader
{
public:
	virtual VertexShaderOutput Main(const Vertex& vertex);

public:
	Matrix4 inverseWorldMatrix;
	Matrix4 worldViewProjMatrix;
	Vector3 lightPosition;
	Vector3 diffuseColor;
	Vector3 ambientColor;
};

class VsTangentSpaceLighting : public VertexShader
{
public:
	virtual VertexShaderOutput Main(const Vertex& vertex);

public:
	Matrix4 worldMatrix;
	Matrix4 inverseWorldMatrix;
	Matrix4 worldViewProjMatrix;

	Vector3 lightPosition;
};

class VsTangentSpaceLightingSc2Uv : public VertexShader
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
