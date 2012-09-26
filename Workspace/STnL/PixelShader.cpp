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
#include "Utilities.h"

Vector4 MyPixelShader::Main( VertexShaderOutput& vertexAttribute )
{
	Vector4& baseColor = baseTexture->Sample(vertexAttribute.texCoord.x, vertexAttribute.texCoord.y);
	Vector4& normal4 = normalTexture->Sample(vertexAttribute.texCoord.x, vertexAttribute.texCoord.y);

	//Vector3 normal(normal4.x * 2.0f - 1.0f, normal4.y * 2.0f - 1.0f, normal4.z * 2.0f - 1.0f);
	//normal.Normalize();
	Vector3 normal(0.0f, 0.0f, 1.0f);

	Vector3 lightDir;
	lightDir.x = vertexAttribute.attribute0.x;
	lightDir.y = vertexAttribute.attribute0.y;
	lightDir.z = vertexAttribute.attribute0.z;
	lightDir.Normalize();

	float angle = normal.Dot(lightDir);
	Saturate(angle);

	baseColor.x *= angle * diffuseColor.x + ambientColor.x;
	baseColor.y *= angle * diffuseColor.y + ambientColor.y;
	baseColor.z *= angle * diffuseColor.z + ambientColor.z;

	return Saturate(baseColor);
}
