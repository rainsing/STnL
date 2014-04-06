/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:38
	filename: 	e:\GitHub\STnL\Workspace\STnL\IndexBuffer.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	IndexBuffer
	file ext:	cpp
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer( void )
{
	m_indices = NULL;
}

IndexBuffer::~IndexBuffer()
{
	if (m_indices)
	{
		delete[] m_indices;
		m_indices = NULL;
	}
}

void IndexBuffer::Initialize( unsigned nIndices )
{
	m_nIndices = nIndices;
	m_indices = new unsigned short[nIndices];
}

unsigned short& IndexBuffer::operator[]( unsigned i )
{
	return *(m_indices + i);
}

unsigned IndexBuffer::Size( void )
{
	return m_nIndices;
}
