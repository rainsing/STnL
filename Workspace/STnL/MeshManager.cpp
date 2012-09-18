/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:58
	filename: 	e:\GitHub\STnL\Workspace\STnL\MeshManager.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	MeshManager
	file ext:	cpp
	author:		Rainisng
	
	purpose:	负责3D mesh的加载和管理
*********************************************************************/
#include "stdafx.h"
#include "MeshManager.h"

#include "ObjMeshLoader.h"

#include "Mesh.h"
#include "Vertex.h"

MeshManager::~MeshManager()
{
	for (unsigned i = 0; i < m_meshList.size(); i++)
	{
		delete m_meshList[i];
		m_meshList[i] = NULL;
	}
}

Mesh* MeshManager::LoadFromFile( const char* fileName )
{
	Mesh* mesh = new Mesh();
	ObjLoader::LoadObjMesh(fileName, mesh);
	m_meshList.push_back(mesh);

	return mesh;
}
