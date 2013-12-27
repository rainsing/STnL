/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:36
	filename: 	e:\GitHub\STnL\Workspace\STnL\VertexBuffer.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	VertexBuffer
	file ext:	cpp
	author:		Rainsing
	
	purpose:	¶¥µãÊý×é
*********************************************************************/
#include "stdafx.h"
#include "VertexBuffer.h"

#include "Vertex.h"

VertexBuffer::VertexBuffer( void )
{
	m_verts = NULL;
}

VertexBuffer::~VertexBuffer()
{
	if (m_verts)
	{
		delete[] m_verts;
		m_verts = NULL;
	}
}

void VertexBuffer::Initialize( unsigned nVerts )
{
	m_nVerts = nVerts;
	m_verts = new Vertex[nVerts];
}

Vertex& VertexBuffer::operator[]( unsigned i )
{
	return *(m_verts + i);
}

unsigned VertexBuffer::Size( void )
{
	return m_nVerts;
}
