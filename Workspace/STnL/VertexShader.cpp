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
#include "Utilities.h"

VertexShaderOutput::VertexShaderOutput( void )
{
	ZeroMemory(this, sizeof(VertexShaderOutput));
}

void Lerp( VertexShaderOutput& out, VertexShaderOutput& va0, VertexShaderOutput& va1, float t )
{
	// Depth should be linearly interpolated.
	out.position.z = (va0.position.z - va1.position.z) * t + va1.position.z;

	// texture coordinate interpolation must be perspective-correct
	float zR0 = 1.0f / va0.position.w;
	float zR1 = 1.0f / va1.position.w;
	float zRt = (zR0 - zR1) * t + zR1;

	Vector2 texCoordR0;
	texCoordR0.x = va0.texCoord.x * zR0;
	texCoordR0.y = va0.texCoord.y * zR0;

	Vector2 texCoordR1;
	texCoordR1.x = va1.texCoord.x * zR1;
	texCoordR1.y = va1.texCoord.y * zR1;

	Vector2 texCoordRt;
	texCoordRt.x = (texCoordR0.x - texCoordR1.x) * t + texCoordR1.x;
	texCoordRt.y = (texCoordR0.y - texCoordR1.y) * t + texCoordR1.y;

	out.position.w = 1.0f / zRt;

	out.texCoord.x = texCoordRt.x * out.position.w;
	out.texCoord.y = texCoordRt.y * out.position.w;

	// linerly interpolate other attributes for performance's sake
	Lerp(out.attribute0, va0.attribute0, va1.attribute0, t);
}

VertexShaderOutput Lerp( VertexShaderOutput& va0, VertexShaderOutput& va1, float t )
{
	VertexShaderOutput out;
	Lerp(out, va0, va1, t);
	return out;
}

VertexShaderOutput VsFixedFunction::Main( const Vertex& vertex )
{
	VertexShaderOutput output;

	// clip space position
	output.position = worldViewProjMatrix.Transform(vertex.position);

	// uv
	output.texCoord.x = vertex.texCoord.x;
	output.texCoord.y = vertex.texCoord.y;

	// lighting
	Vector4& objSpaceLightPos = inverseWorldMatrix.Transform(lightPosition);

	Vector3 objSpaceLightDir;
	objSpaceLightDir.x = objSpaceLightPos.x;
	objSpaceLightDir.y = objSpaceLightPos.y;
	objSpaceLightDir.z = objSpaceLightPos.z;
	objSpaceLightDir = objSpaceLightDir - vertex.position;
	objSpaceLightDir.Normalize();

	float angle = vertex.normal.Dot(objSpaceLightDir);
	Saturate(angle);

	output.attribute0 = diffuseColor * angle + ambientColor;

	return output;
}

VertexShaderOutput VsFixedFunctionAltUv::Main( const Vertex& vertex )
{
	VertexShaderOutput output;

	// clip space position
	output.position = worldViewProjMatrix.Transform(vertex.position);

	// StarCraft 2 uses a strange UV coordinate system
	output.texCoord.x = vertex.texCoord.x * 2.0f;
	output.texCoord.y = 1.0f - vertex.texCoord.y;

	// lighting
	Vector4& objSpaceLightPos = inverseWorldMatrix.Transform(lightPosition);

	Vector3 objSpaceLightDir;
	objSpaceLightDir.x = objSpaceLightPos.x;
	objSpaceLightDir.y = objSpaceLightPos.y;
	objSpaceLightDir.z = objSpaceLightPos.z;
	objSpaceLightDir = objSpaceLightDir - vertex.position;
	objSpaceLightDir.Normalize();

	float angle = vertex.normal.Dot(objSpaceLightDir);
	Saturate(angle);

	output.attribute0 = diffuseColor * angle + ambientColor;

	return output;
}

VertexShaderOutput VsTangentSpaceLighting::Main( const Vertex& vertex )
{
	VertexShaderOutput output;

	// clip space position
	output.position = worldViewProjMatrix.Transform(vertex.position);

	// uv
	output.texCoord.x = vertex.texCoord.x;
	output.texCoord.y = vertex.texCoord.y;

	// normal vector
	Vector4& objSpaceLightPos = inverseWorldMatrix.Transform(lightPosition);

	// light direction in tangent space
	Vector3 objSpaceLightDir;
	objSpaceLightDir.x = objSpaceLightPos.x;
	objSpaceLightDir.y = objSpaceLightPos.y;
	objSpaceLightDir.z = objSpaceLightPos.z;

	objSpaceLightDir = objSpaceLightDir - vertex.position;

	output.attribute0.x = vertex.tangent.Dot(objSpaceLightDir);
	output.attribute0.y = vertex.binormal.Dot(objSpaceLightDir);
	output.attribute0.z = vertex.normal.Dot(objSpaceLightDir);

	return output;
}

VertexShaderOutput VsTangentSpaceLightingSc2Uv::Main( const Vertex& vertex )
{
	VertexShaderOutput output;

	// clip space position
	output.position = worldViewProjMatrix.Transform(vertex.position);

	// StarCraft 2 uses a strange UV coordinate system
	output.texCoord.x = vertex.texCoord.x * 2.0f;
	output.texCoord.y = 1.0f - vertex.texCoord.y;

	// normal vector
	Vector4& objSpaceLightPos = inverseWorldMatrix.Transform(lightPosition);

	// light direction in tangent space
	Vector3 objSpaceLightDir;
	objSpaceLightDir.x = objSpaceLightPos.x;
	objSpaceLightDir.y = objSpaceLightPos.y;
	objSpaceLightDir.z = objSpaceLightPos.z;

	objSpaceLightDir = objSpaceLightDir - vertex.position;

	output.attribute0.x = vertex.tangent.Dot(objSpaceLightDir);
	output.attribute0.y = vertex.binormal.Dot(objSpaceLightDir);
	output.attribute0.z = vertex.normal.Dot(objSpaceLightDir);

	return output;
}
