/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:14
	filename: 	e:\GitHub\STnL\Workspace\STnL\Matrix.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Matrix
	file ext:	h
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#ifndef Matrix_h__
#define Matrix_h__

#include "Vector.h"

class Matrix4
{
public:
	Matrix4();

	Matrix4(
		float _m11, float _m12, float _m13, float _m14,
		float _m21, float _m22, float _m23, float _m24,
		float _m31, float _m32, float _m33, float _m34,
		float _m41, float _m42, float _m43, float _m44
	);

	Vector4 Transform(const Vector4& vec);

	static Matrix4 IDENTITY;

	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;
};

Matrix4& MakeRotationMatrixX(Matrix4& out, float angle);
Matrix4& MakeProjectionMatrix(Matrix4& out, float zn, float zf, float fov, float aspect);
void MatrixMultiply(Matrix4& out, const Matrix4& a, const Matrix4& b);

#endif // Matrix_h__
