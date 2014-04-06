/********************************************************************
	created:	2012/09/24
	created:	24:9:2012   10:18
	filename: 	e:\GitHub\STnL\Workspace\STnL\PixelShader.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	PixelShader
	file ext:	h
	author:		Rainsing
	
	purpose:	Software pixel shader
*********************************************************************/
#ifndef PixelShader_h__
#define PixelShader_h__

#include "Vector.h"

class VertexShaderOutput;
class Texture;

class PixelShader
{
public:

	// The entry point of the pixel shader. 
	// Output from the vertex shader is interpolated and passed in as 'vertexAttribute'.
	// Any uniforms / constants or texture samplers can be defined as data member in derived classes.
	// The function should return the final pixel color.
	virtual Vector4 Main(VertexShaderOutput& vertexAttribute) = 0;
};

class PsFixedFunction : public PixelShader
{
public:
	virtual Vector4 Main(VertexShaderOutput& vertexAttribute);

public:
	Texture* baseTexture;
};

class PsNormalMap : public PixelShader
{
public:
	virtual Vector4 Main(VertexShaderOutput& vertexAttribute);

public:
	Texture* baseTexture;
	Texture* normalTexture;

	Vector3 lightColor;
	Vector3 ambientColor;
};

class PsToonLighting : public PixelShader
{
public:
	virtual Vector4 Main(VertexShaderOutput& vertexAttribute);

public:
	Texture* baseTexture;

	Vector3 lightColor;
	Vector3 ambientColor;
};

#endif // PixelShader_h__
