/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:35
	filename: 	e:\GitHub\STnL\Workspace\STnL\VertexBuffer.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	VertexBuffer
	file ext:	h
	author:		Rainsing
	
	purpose:	¶¥µãÊý×é
*********************************************************************/
#ifndef VertexBuffer_h__
#define VertexBuffer_h__

class Vertex;

class VertexBuffer
{
public:
	VertexBuffer(void);
	~VertexBuffer();

	void Initialize(unsigned nVerts);
	Vertex& operator [](unsigned i);
	unsigned Size(void);

private:
	unsigned m_nVerts;
	Vertex* m_verts;
};

#endif // VertexBuffer_h__
