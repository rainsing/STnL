/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:11
	filename: 	e:\GitHub\STnL\Workspace\STnL\SceneObject.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	SceneObject
	file ext:	cpp
	author:		Rainsing
	
	purpose:	表示3D场景中的一个物体
*********************************************************************/
#include "stdafx.h"
#include "SceneObject.h"

SceneObject::SceneObject( Mesh* mesh, Material* material )
{
	m_mesh = mesh;
	m_material = material;
	m_worldMatrix = Matrix4::IDENTITY;
}

void SceneObject::LocalRotate( float x, float y, float z )
{
	Vector3 translation;
	translation.x = m_worldMatrix.m41;
	translation.y = m_worldMatrix.m42;
	translation.z = m_worldMatrix.m43;

	m_worldMatrix.m41 = m_worldMatrix.m42 = m_worldMatrix.m43 = 0;

	Matrix4 rotationMatrix;

	MakeRotationMatrixX(rotationMatrix, x);
	MatrixMultiply(m_worldMatrix, m_worldMatrix, rotationMatrix);

	MakeRotationMatrixY(rotationMatrix, y);
	MatrixMultiply(m_worldMatrix, m_worldMatrix, rotationMatrix);

	MakeRotationMatrixZ(rotationMatrix, z);
	MatrixMultiply(m_worldMatrix, m_worldMatrix, rotationMatrix);

	m_worldMatrix.m41 = translation.x;
	m_worldMatrix.m42 = translation.y;
	m_worldMatrix.m43 = translation.z;
}
