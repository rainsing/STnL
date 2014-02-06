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

#include "Utilities.h"

const Vector3 Vector3::ZERO(0.0f, 0.0f, 0.0f);

Vector4 Lerp(Vector4& v0, Vector4& v1, float t)
{
	Vector4 out;

	out.x = (v0.x - v1.x) * t + v1.x;
	out.y = (v0.y - v1.y) * t + v1.y;
	out.z = (v0.z - v1.z) * t + v1.z;
	out.w = (v0.w - v1.w) * t + v1.w;

	return out;
}

void Lerp(Vector4& out, Vector4& v0, Vector4& v1, float t)
{
	out.x = (v0.x - v1.x) * t + v1.x;
	out.y = (v0.y - v1.y) * t + v1.y;
	out.z = (v0.z - v1.z) * t + v1.z;
	out.w = (v0.w - v1.w) * t + v1.w;
}

Vector4& Clamp(Vector4& value, float minValue, float maxValue)
{
	Clamp(value.x, minValue, maxValue);
	Clamp(value.y, minValue, maxValue);
	Clamp(value.z, minValue, maxValue);
	Clamp(value.w, minValue, maxValue);

	return value;
}

Vector4& Saturate( Vector4& value )
{
	return Clamp(value, 0.0f, 1.0f);
}
