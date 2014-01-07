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

Vector4 PsFixedFunction::Main( VertexShaderOutput& vertexAttribute )
{
	Vector4& baseColor = baseTexture->Sample(vertexAttribute.texCoord.x, vertexAttribute.texCoord.y);

	baseColor.x *= vertexAttribute.attribute0.x;
	baseColor.y *= vertexAttribute.attribute0.y;
	baseColor.z *= vertexAttribute.attribute0.z;

	return Saturate(baseColor);
}

Vector4 PsNormalMap::Main( VertexShaderOutput& vertexAttribute )
{
	Vector4& baseColor = baseTexture->Sample(vertexAttribute.texCoord.x, vertexAttribute.texCoord.y);
	Vector4& normal4 = normalTexture->Sample(vertexAttribute.texCoord.x, vertexAttribute.texCoord.y);

	Vector3 normal(normal4.x * 2.0f - 1.0f, normal4.y * 2.0f - 1.0f, normal4.z * 2.0f - 1.0f);
	normal.Normalize();

	Vector3 lightDir;
	lightDir.x = vertexAttribute.attribute0.x;
	lightDir.y = vertexAttribute.attribute0.y;
	lightDir.z = vertexAttribute.attribute0.z;
	lightDir.Normalize();

	float angle = normal.Dot(lightDir);
	Saturate(angle);

	baseColor.x *= angle * lightColor.x + ambientColor.x;
	baseColor.y *= angle * lightColor.y + ambientColor.y;
	baseColor.z *= angle * lightColor.z + ambientColor.z;

	return Saturate(baseColor);
}

Vector4 PsToonLighting::Main( VertexShaderOutput& vertexAttribute )
{
	Vector4& baseColor = baseTexture->Sample(vertexAttribute.texCoord.x, vertexAttribute.texCoord.y);

    /*Vector3 normal(0.0f, 0.0f, 1.0f);

    Vector3 lightDir;
    lightDir.x = vertexAttribute.attribute0.x;
    lightDir.y = vertexAttribute.attribute0.y;
    lightDir.z = vertexAttribute.attribute0.z;
    lightDir.Normalize();

    float angle = normal.Dot(lightDir);
    Saturate(angle);

    Vector3 lighting = lightColor * angle + ambientColor * 2.0f;*/
    Vector3 lighting;
    lighting.x = vertexAttribute.attribute0.x;
    lighting.y = vertexAttribute.attribute0.y;
    lighting.z = vertexAttribute.attribute0.z;

	// 把光照离散化，达到卡通的效果
	lighting.x -= fmod(lighting.x, 0.2f);
	lighting.y -= fmod(lighting.y, 0.2f);
	lighting.z -= fmod(lighting.z, 0.2f);

	baseColor.x *= lighting.x * 2.0f;
	baseColor.y *= lighting.y * 2.0f;
	baseColor.z *= lighting.z * 2.0f;

	return Saturate(baseColor);
}
