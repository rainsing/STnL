/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:14
	filename: 	e:\GitHub\STnL\Workspace\STnL\Matrix.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Matrix
	file ext:	cpp
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "Matrix.h"

#include "Utilities.h"

#include <math.h>

const Matrix4 Matrix4::IDENTITY(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

Matrix4::Matrix4()
{

}

Matrix4::Matrix4(float _m11, float _m12, float _m13, float _m14, 
				 float _m21, float _m22, float _m23, float _m24, 
				 float _m31, float _m32, float _m33, float _m34, 
				 float _m41, float _m42, float _m43, float _m44 )
{
	m11 = _m11; m12 = _m12; m13 = _m13; m14 = _m14;
	m21 = _m21; m22 = _m22; m23 = _m23; m24 = _m24;
	m31 = _m31; m32 = _m32; m33 = _m33; m34 = _m34;
	m41 = _m41; m42 = _m42; m43 = _m43; m44 = _m44;
}

Vector4 Matrix4::Transform( const Vector4& vec )
{
	Vector4 out;

	out.x = vec.x * m11 + vec.y * m21 + vec.z * m31 + vec.w * m41;
	out.y = vec.x * m12 + vec.y * m22 + vec.z * m32 + vec.w * m42;
	out.z = vec.x * m13 + vec.y * m23 + vec.z * m33 + vec.w * m43;
	out.w = vec.x * m14 + vec.y * m24 + vec.z * m34 + vec.w * m44;

	return out;
}

void MatrixMultiply( Matrix4& out, const Matrix4& a, const Matrix4& b )
{
	Matrix4 temp;

	temp.m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41;
	temp.m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42;
	temp.m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43;
	temp.m14 = a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44;

	temp.m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41;
	temp.m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42;
	temp.m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43;
	temp.m24 = a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44;

	temp.m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41;
	temp.m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42;
	temp.m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43;
	temp.m34 = a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44;

	temp.m41 = a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41;
	temp.m42 = a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42;
	temp.m43 = a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43;
	temp.m44 = a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44;

	out = temp;
}

void MatrixTranspose( Matrix4& out, const Matrix4& a )
{
	out.m11 = a.m11; out.m12 = a.m21; out.m13 = a.m31; out.m14 = a.m41;
	out.m21 = a.m12; out.m22 = a.m22; out.m23 = a.m32; out.m24 = a.m42;
	out.m31 = a.m13; out.m32 = a.m23; out.m33 = a.m33; out.m34 = a.m43;
	out.m41 = a.m14; out.m42 = a.m24; out.m43 = a.m34; out.m44 = a.m44;
}

Matrix4& MakeProjectionMatrix( Matrix4& out, float zn, float zf, float fov, float aspect )
{
	ZeroMemory(&out, sizeof(Matrix4));

	float yScale = 1.0f / tan(TO_RADIAN(fov) / 2.0f);
	float xScale = yScale / aspect;
	float zScale = zf / (zf - zn);

	out.m11 = xScale;
	out.m22 = yScale;
	out.m33 = zScale;
	out.m34 = 1.0f;
	out.m43 = -zn * zScale;

	return out;
}

Matrix4& MakeRotationMatrixX( Matrix4& out, float angle )
{
	ZeroMemory(&out, sizeof(Matrix4));

	float radian = TO_RADIAN(angle);
	float sine = sin(radian);
	float cosine = cos(radian);

	out.m11 = 1.0f;
	out.m22 = cosine;
	out.m23 = sine;
	out.m32 = -sine;
	out.m33 = cosine;
	out.m44 = 1.0f;

	return out;
}

Matrix4& MakeRotationMatrixY( Matrix4& out, float angle )
{
	ZeroMemory(&out, sizeof(Matrix4));

	float radian = TO_RADIAN(angle);
	float sine = sin(radian);
	float cosine = cos(radian);

	out.m11 = cosine;
	out.m13 = -sine;
	out.m22 = 1.0f;
	out.m31 = sine;
	out.m33 = cosine;
	out.m44 = 1.0f;

	return out;
}

Matrix4& MakeRotationMatrixZ( Matrix4& out, float angle )
{
	ZeroMemory(&out, sizeof(Matrix4));

	float radian = TO_RADIAN(angle);
	float sine = sin(radian);
	float cosine = cos(radian);

	out.m11 = cosine;
	out.m12 = sine;
	out.m21 = -sine;
	out.m22 = cosine;
	out.m33 = 1.0f;
	out.m44 = 1.0f;

	return out;
}
