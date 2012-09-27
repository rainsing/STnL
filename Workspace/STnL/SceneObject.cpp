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
	m_rotationMatrix = Matrix4::IDENTITY;
	m_hide = true;
}

void SceneObject::LocalRotate( float x, float y, float z )
{
	Matrix4 rotationMatrix;

	MakeRotationMatrixX(rotationMatrix, x);
	MatrixMultiply(m_rotationMatrix, m_rotationMatrix, rotationMatrix);

	MakeRotationMatrixY(rotationMatrix, y);
	MatrixMultiply(m_rotationMatrix, m_rotationMatrix, rotationMatrix);

	MakeRotationMatrixZ(rotationMatrix, z);
	MatrixMultiply(m_rotationMatrix, m_rotationMatrix, rotationMatrix);
}

void SceneObject::ResetRotation( void )
{
	m_rotationMatrix = Matrix4::IDENTITY;
}

void SceneObject::GetWorldMatrix( Matrix4& out )
{
	MatrixMultiply(out, m_rotationMatrix, m_worldMatrix);
}
