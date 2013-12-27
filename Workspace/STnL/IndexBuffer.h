/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   10:37
	filename: 	e:\GitHub\STnL\Workspace\STnL\IndexBuffer.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	IndexBuffer
	file ext:	h
	author:		Rainsing
	
	purpose:	顶点索引数组
*********************************************************************/
#ifndef IndexBuffer_h__
#define IndexBuffer_h__

class IndexBuffer
{
public:
	IndexBuffer(void);
	~IndexBuffer();

	void Initialize(unsigned nIndices);
	unsigned short& operator [](unsigned i);
	unsigned Size(void);

private:
	unsigned m_nIndices;
	unsigned short* m_indices;
};

#endif // IndexBuffer_h__
