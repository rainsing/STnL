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
	// TODO: 先手写个cube在这儿
	Mesh* mesh = new Mesh();
	m_meshList.push_back(mesh);

	mesh->m_vb.Initialize(8);

	mesh->m_vb[0].position.x = -1.0f;
	mesh->m_vb[0].position.y = -1.0f;
	mesh->m_vb[0].position.z = -1.0f;

	mesh->m_vb[1].position.x =  1.0f;
	mesh->m_vb[1].position.y = -1.0f;
	mesh->m_vb[1].position.z = -1.0f;

	mesh->m_vb[2].position.x =  1.0f;
	mesh->m_vb[2].position.y = -1.0f;
	mesh->m_vb[2].position.z =  1.0f;

	mesh->m_vb[3].position.x = -1.0f;
	mesh->m_vb[3].position.y = -1.0f;
	mesh->m_vb[3].position.z =  1.0f;

	mesh->m_vb[4].position.x = -1.0f / 2;
	mesh->m_vb[4].position.y =  1.0f;
	mesh->m_vb[4].position.z = -1.0f / 2;

	mesh->m_vb[5].position.x =  1.0f / 2;
	mesh->m_vb[5].position.y =  1.0f;
	mesh->m_vb[5].position.z = -1.0f / 2;

	mesh->m_vb[6].position.x =  1.0f / 2;
	mesh->m_vb[6].position.y =  1.0f;
	mesh->m_vb[6].position.z =  1.0f / 2;

	mesh->m_vb[7].position.x = -1.0f / 2;
	mesh->m_vb[7].position.y =  1.0f;
	mesh->m_vb[7].position.z =  1.0f / 2;

	mesh->m_ib.Initialize(36);

	mesh->m_ib[0] = 0;
	mesh->m_ib[1] = 4;
	mesh->m_ib[2] = 1;
	mesh->m_ib[3] = 4;
	mesh->m_ib[4] = 5;
	mesh->m_ib[5] = 1;

	mesh->m_ib[6] = 4;
	mesh->m_ib[7] = 7;
	mesh->m_ib[8] = 5;
	mesh->m_ib[9] = 5;
	mesh->m_ib[10] = 7;
	mesh->m_ib[11] = 6;

	mesh->m_ib[12] = 5;
	mesh->m_ib[13] = 6;
	mesh->m_ib[14] = 1;
	mesh->m_ib[15] = 1;
	mesh->m_ib[16] = 6;
	mesh->m_ib[17] = 2;

	mesh->m_ib[18] = 0;
	mesh->m_ib[19] = 7;
	mesh->m_ib[20] = 4;
	mesh->m_ib[21] = 0;
	mesh->m_ib[22] = 3;
	mesh->m_ib[23] = 7;

	mesh->m_ib[24] = 2;
	mesh->m_ib[25] = 7;
	mesh->m_ib[26] = 3;
	mesh->m_ib[27] = 2;
	mesh->m_ib[28] = 6;
	mesh->m_ib[29] = 7;

	mesh->m_ib[30] = 1;
	mesh->m_ib[31] = 2;
	mesh->m_ib[32] = 0;
	mesh->m_ib[33] = 0;
	mesh->m_ib[34] = 2;
	mesh->m_ib[35] = 3;

	return mesh;
}
