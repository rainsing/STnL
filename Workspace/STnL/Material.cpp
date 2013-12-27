/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   13:13
	filename: 	e:\GitHub\STnL\Workspace\STnL\Material.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Material
	file ext:	cpp
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "Material.h"

#include "Utilities.h"

MaterialManager::MaterialManager( void )
{

}

MaterialManager::~MaterialManager()
{
	for (unsigned i = 0; i < m_matList.size(); i++)
	{
		SAFE_DELETE(m_matList[i]);
	}
}

Material* MaterialManager::CreateMaterial( void )
{
	Material* mat = new Material();
	m_matList.push_back(mat);
	return mat;
}
