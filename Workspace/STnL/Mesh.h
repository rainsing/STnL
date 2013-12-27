/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:31
	filename: 	e:\GitHub\STnL\Workspace\STnL\Mesh.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Mesh
	file ext:	h
	author:		Rainsing
	
	purpose:	3D模型网格数据
*********************************************************************/
#ifndef Mesh_h__
#define Mesh_h__

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
	friend class MeshManager;

public:
	inline VertexBuffer* GetVertexBuffer(void);
	inline IndexBuffer* GetIndexBuffer(void);

private:
	VertexBuffer m_vb;
	IndexBuffer m_ib;
};

VertexBuffer* Mesh::GetVertexBuffer(void)
{
	return &m_vb;
}

IndexBuffer* Mesh::GetIndexBuffer(void)
{
	return &m_ib;
}

#endif // Mesh_h__
