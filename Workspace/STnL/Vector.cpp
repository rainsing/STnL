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

const Vector3 Vector3::ZERO(0.0f, 0.0f, 0.0f);

Vector4 Lerp(Vector4& v0, Vector4& v1, float t)
{
	Vector4 out;
	out.x = v0.x * t + v1.x * (1.0f - t);
	out.y = v0.y * t + v1.y * (1.0f - t);
	out.z = v0.z * t + v1.z * (1.0f - t);
	out.w = v0.w * t + v1.w * (1.0f - t);
	return out;
}

void Lerp(Vector4& out, Vector4& v0, Vector4& v1, float t)
{
	out.x = v0.x * t + v1.x * (1.0f - t);
	out.y = v0.y * t + v1.y * (1.0f - t);
	out.z = v0.z * t + v1.z * (1.0f - t);
	out.w = v0.w * t + v1.w * (1.0f - t);
}
