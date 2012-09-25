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

VertexShaderOutput MyVertexShader::Main( const Vertex& vertex )
{
	VertexShaderOutput output;

	// clip space position
	output.position = worldViewProjMatrix.Transform(vertex.position);

	// uv
	output.texCoord.x = vertex.texCoord.x;
	output.texCoord.y = vertex.texCoord.y;

	// normal vector
	//output.attribute0 = vertex.normal;
	Vector4& objSpaceLightPos = inverseWorldMatrix.Transform(lightPosition);

	Vector3 objSpaceLightDir;
	objSpaceLightDir.x = objSpaceLightPos.x;
	objSpaceLightDir.y = objSpaceLightPos.y;
	objSpaceLightDir.z = objSpaceLightPos.z;

	objSpaceLightDir = objSpaceLightDir - vertex.position;

	output.attribute0.x = vertex.tangent.Dot(objSpaceLightDir);
	output.attribute0.y = vertex.binormal.Dot(objSpaceLightDir);
	output.attribute0.z = vertex.normal.Dot(objSpaceLightDir);

	// light direction
	//output.attribute1 = inverseWorldMatrix.Transform(lightPosition)- vertex.position;

	return output;
}

VertexShaderOutput Lerp( VertexShaderOutput& va0, VertexShaderOutput& va1, float t )
{
	VertexShaderOutput out;

	// Depth should be linearly interpolated.
	out.position.z = (va0.position.z - va1.position.z) * t + va1.position.z;

	// other interpolations must be perspective-correct
	float zR0 = 1.0f / va0.position.w;
	Vector4 attr0R0(va0.texCoord.x * zR0, va0.texCoord.y * zR0, va0.texCoord.z * zR0, va0.texCoord.w * zR0);

	float zR1 = 1.0f / va1.position.w;
	Vector4 attr0R1(va1.texCoord.x * zR1, va1.texCoord.y * zR1, va1.texCoord.z * zR1, va1.texCoord.w * zR1);

	float zRt = (zR0 - zR1) * t + zR1;
	Vector4 attr0Rt;

	Lerp(attr0Rt, attr0R0, attr0R1, t);

	out.position.w = 1.0f / zRt;

	out.texCoord.x = attr0Rt.x * out.position.w;
	out.texCoord.y = attr0Rt.y * out.position.w;
	out.texCoord.z = attr0Rt.z * out.position.w;
	out.texCoord.w = attr0Rt.w * out.position.w;

	// -----------------------------------------
	Lerp(out.attribute0, va0.attribute0, va1.attribute0, t);
	Lerp(out.attribute1, va0.attribute1, va1.attribute1, t);

	return out;
}

void Lerp( VertexShaderOutput& out, VertexShaderOutput& va0, VertexShaderOutput& va1, float t )
{
	// Depth should be linearly interpolated.
	out.position.z = (va0.position.z - va1.position.z) * t + va1.position.z;

	// other interpolations must be perspective-correct
	float zR0 = 1.0f / va0.position.w;
	Vector4 attr0R0(va0.texCoord.x * zR0, va0.texCoord.y * zR0, va0.texCoord.z * zR0, va0.texCoord.w * zR0);

	float zR1 = 1.0f / va1.position.w;
	Vector4 attr0R1(va1.texCoord.x * zR1, va1.texCoord.y * zR1, va1.texCoord.z * zR1, va1.texCoord.w * zR1);

	float zRt = (zR0 - zR1) * t + zR1;
	Vector4 attr0Rt;

	Lerp(attr0Rt, attr0R0, attr0R1, t);

	out.position.w = 1.0f / zRt;

	out.texCoord.x = attr0Rt.x * out.position.w;
	out.texCoord.y = attr0Rt.y * out.position.w;
	out.texCoord.z = attr0Rt.z * out.position.w;
	out.texCoord.w = attr0Rt.w * out.position.w;

	// -----------------------------------------
	Lerp(out.attribute0, va0.attribute0, va1.attribute0, t);
	Lerp(out.attribute1, va0.attribute1, va1.attribute1, t);

	/*float zR0 = 1.0f / va0.position.w;
	float zR1 = 1.0f / va1.position.w;
	float zRt = (zR0 - zR1) * t + zR1;
	out.position.w = 1.0f / zRt;

	__m128 packedScalar;

	__m128 attr0R0 = _mm_set_ps(va0.texCoord.w, va0.texCoord.z, va0.texCoord.y, va0.texCoord.x);

	packedScalar = _mm_set_ps1(zR0);
	attr0R0 = _mm_mul_ps(attr0R0, packedScalar);

	__m128 attr0R1 = _mm_set_ps(va1.texCoord.w, va1.texCoord.z, va1.texCoord.y, va1.texCoord.x);

	packedScalar = _mm_set_ps1(zR1);
	attr0R1 = _mm_mul_ps(attr0R1, packedScalar);

	__m128 attr0Rt;

	packedScalar = _mm_set_ps1(t);
	Lerp_SSE(attr0Rt, attr0R0, attr0R1, packedScalar);
	
	packedScalar = _mm_set_ps1(out.position.w);
	attr0Rt = _mm_mul_ps(attr0Rt, packedScalar);

	_mm_storeu_ps(&out.texCoord.x, attr0Rt);*/
}
