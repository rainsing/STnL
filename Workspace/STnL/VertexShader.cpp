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
	output.atrribute0.x = vertex.texCoord.x;
	output.atrribute0.y = vertex.texCoord.y;

	/*Vector4 worldSpaceNormal = worldMatrix.Transform(vertex.normal);

	Vector3 worldSpaceNormal3;
	worldSpaceNormal3.x = worldSpaceNormal.x;
	worldSpaceNormal3.y = worldSpaceNormal.y;
	worldSpaceNormal3.z = worldSpaceNormal.z;

	float angle = worldSpaceNormal3.Dot(Vector3(0.0f, 1.0f, 0.0f));
	if (angle < 0.0f)
	{
		angle = 0.0f;
	}
	output.atrribute0 = Vector3(0.0f, 1.0f, 0.0f) * angle;*/

	return output;
}

VertexShaderOutput Lerp( VertexShaderOutput& va0, VertexShaderOutput& va1, float t )
{
	VertexShaderOutput out;

	// Depth should be linearly interpolated.
	out.position.z = (va0.position.z - va1.position.z) * t + va1.position.z;

	// other interpolations must be perspective-correct
	float zR0 = 1.0f / va0.position.w;
	Vector4 attr0R0(va0.atrribute0.x * zR0, va0.atrribute0.y * zR0, va0.atrribute0.z * zR0, va0.atrribute0.w * zR0);

	float zR1 = 1.0f / va1.position.w;
	Vector4 attr0R1(va1.atrribute0.x * zR1, va1.atrribute0.y * zR1, va1.atrribute0.z * zR1, va1.atrribute0.w * zR1);

	float zRt = (zR0 - zR1) * t + zR1;
	Vector4 attr0Rt;

	Lerp(attr0Rt, attr0R0, attr0R1, t);

	out.position.w = 1.0f / zRt;

	out.atrribute0.x = attr0Rt.x * out.position.w;
	out.atrribute0.y = attr0Rt.y * out.position.w;
	out.atrribute0.z = attr0Rt.z * out.position.w;
	out.atrribute0.w = attr0Rt.w * out.position.w;

	return out;
}

void Lerp( VertexShaderOutput& out, VertexShaderOutput& va0, VertexShaderOutput& va1, float t )
{
	// Depth should be linearly interpolated.
	out.position.z = (va0.position.z - va1.position.z) * t + va1.position.z;

	// other interpolations must be perspective-correct
	float zR0 = 1.0f / va0.position.w;
	Vector4 attr0R0(va0.atrribute0.x * zR0, va0.atrribute0.y * zR0, va0.atrribute0.z * zR0, va0.atrribute0.w * zR0);

	float zR1 = 1.0f / va1.position.w;
	Vector4 attr0R1(va1.atrribute0.x * zR1, va1.atrribute0.y * zR1, va1.atrribute0.z * zR1, va1.atrribute0.w * zR1);

	float zRt = (zR0 - zR1) * t + zR1;
	Vector4 attr0Rt;

	Lerp(attr0Rt, attr0R0, attr0R1, t);

	out.position.w = 1.0f / zRt;

	out.atrribute0.x = attr0Rt.x * out.position.w;
	out.atrribute0.y = attr0Rt.y * out.position.w;
	out.atrribute0.z = attr0Rt.z * out.position.w;
	out.atrribute0.w = attr0Rt.w * out.position.w;

	/*float zR0 = 1.0f / va0.position.w;
	float zR1 = 1.0f / va1.position.w;
	float zRt = (zR0 - zR1) * t + zR1;
	out.position.w = 1.0f / zRt;

	__m128 packedScalar;

	__m128 attr0R0 = _mm_set_ps(va0.atrribute0.w, va0.atrribute0.z, va0.atrribute0.y, va0.atrribute0.x);

	packedScalar = _mm_set_ps1(zR0);
	attr0R0 = _mm_mul_ps(attr0R0, packedScalar);

	__m128 attr0R1 = _mm_set_ps(va1.atrribute0.w, va1.atrribute0.z, va1.atrribute0.y, va1.atrribute0.x);

	packedScalar = _mm_set_ps1(zR1);
	attr0R1 = _mm_mul_ps(attr0R1, packedScalar);

	__m128 attr0Rt;

	packedScalar = _mm_set_ps1(t);
	Lerp_SSE(attr0Rt, attr0R0, attr0R1, packedScalar);
	
	packedScalar = _mm_set_ps1(out.position.w);
	attr0Rt = _mm_mul_ps(attr0Rt, packedScalar);

	_mm_storeu_ps(&out.atrribute0.x, attr0Rt);*/
}
