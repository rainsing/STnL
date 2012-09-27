/********************************************************************
	created:	2012/09/27
	created:	27:9:2012   11:41
	filename: 	e:\GitHub\STnL\Workspace\STnL\PointLight.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	PointLight
	file ext:	h
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#ifndef PointLight_h__
#define PointLight_h__

#include "Vector.h"

class PointLight
{
public:
	Vector3 position;
	Vector3 diffuseColor;
	Vector3 ambientColor;
};

#endif // PointLight_h__
