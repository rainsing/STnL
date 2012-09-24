/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   11:32
	filename: 	e:\GitHub\STnL\Workspace\STnL\Vector.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Vector
	file ext:	cpp
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "Vector.h"

#include <xmmintrin.h>

const Vector3 Vector3::ZERO(0.0f, 0.0f, 0.0f);

Vector4 Lerp(Vector4& v0, Vector4& v1, float t)
{
	Vector4 out;

	out.x = (v0.x - v1.x) * t + v1.x;
	out.y = (v0.y - v1.y) * t + v1.y;
	out.z = (v0.z - v1.z) * t + v1.z;
	out.w = (v0.w - v1.w) * t + v1.w;

	/*__m128 m128_0 = _mm_set_ps(v0.w, v0.z, v0.y, v0.x);
	__m128 m128_1 = _mm_set_ps(v1.w, v1.z, v1.y, v1.x);
	__m128 packedT = _mm_set_ps1(t);

	m128_0 = _mm_sub_ps(m128_0, m128_1);
	m128_0 = _mm_mul_ps(m128_0, packedT);
	m128_0 = _mm_add_ps(m128_0, m128_1);

	_mm_storeu_ps(&out.x, m128_0);*/

	return out;
}

void Lerp(Vector4& out, Vector4& v0, Vector4& v1, float t)
{
	out.x = (v0.x - v1.x) * t + v1.x;
	out.y = (v0.y - v1.y) * t + v1.y;
	out.z = (v0.z - v1.z) * t + v1.z;
	out.w = (v0.w - v1.w) * t + v1.w;

	/*__m128 m128_0 = _mm_set_ps(v0.w, v0.z, v0.y, v0.x);
	__m128 m128_1 = _mm_set_ps(v1.w, v1.z, v1.y, v1.x);
	__m128 packedT = _mm_set_ps1(t);

	m128_0 = _mm_sub_ps(m128_0, m128_1);
	m128_0 = _mm_mul_ps(m128_0, packedT);
	m128_0 = _mm_add_ps(m128_0, m128_1);

	_mm_storeu_ps(&out.x, m128_0);*/
}
