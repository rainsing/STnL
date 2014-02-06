/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:57
	filename: 	e:\GitHub\STnL\Workspace\STnL\MeshManager.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	MeshManager
	file ext:	h
	author:		Rainsing
	
	purpose:	The mesh manager is responsible for loading meshes from file
                and maintaining a list of all loaded meshes.
*********************************************************************/
#ifndef MeshManager_h__
#define MeshManager_h__

class Mesh;

typedef std::vector<Mesh*> MeshList;

class MeshManager
{
public:
	~MeshManager();

	Mesh* LoadFromFile(const char* fileName);

private:
	MeshList m_meshList;
};

#endif // MeshManager_h__
