/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:10
	filename: 	e:\GitHub\STnL\Workspace\STnL\SceneObject.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	SceneObject
	file ext:	h
	author:		Rainsing
	
	purpose:	��ʾ3D�����е�һ������
*********************************************************************/
#ifndef SceneObject_h__
#define SceneObject_h__

#include "Matrix.h"

class Mesh;
class Material;

class SceneObject
{
public:
	SceneObject(Mesh* mesh, Material* material);

	void LocalRotate(float x, float y, float z);
	inline Mesh* GetMesh(void);

public:
	Matrix4 m_worldMatrix;

private:
	Mesh* m_mesh;
	Material* m_material;
};

Mesh* SceneObject::GetMesh( void )
{
	return m_mesh;
}

#endif // SceneObject_h__
