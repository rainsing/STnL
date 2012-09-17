/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   15:35
	filename: 	e:\GitHub\STnL\Workspace\STnL\RenderUnit.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	RenderUnit
	file ext:	cpp
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "RenderUnit.h"

#include "VertexShader.h"

RenderUnit::RenderUnit( void )
{
	m_vb = NULL;
	m_ib = NULL;
	m_vs = NULL;
}

RenderUnit::~RenderUnit()
{
	if (m_vs)
	{
		delete m_vs;
		m_vs = NULL;
	}
}
