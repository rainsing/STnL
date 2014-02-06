/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:11
	filename: 	e:\GitHub\STnL\Workspace\STnL\SceneObject.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	SceneObject
	file ext:	cpp
	author:		Rainsing
	
	purpose:	A visible object in the scene.
*********************************************************************/
#include "stdafx.h"
#include "SceneObject.h"

SceneObject::SceneObject( Mesh* mesh, Material* material )
{
	m_mesh = mesh;
	m_material = material;
	m_worldMatrix = Matrix4::IDENTITY;
	m_hide = true;
}

void SceneObject::GetWorldMatrix( Matrix4& out )
{
    out = m_worldMatrix;
}
