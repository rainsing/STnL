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
	MakeRotationMatrixX(m_worldMatrix, x);
}
