/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   15:34
	filename: 	e:\GitHub\STnL\Workspace\STnL\RenderUnit.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	RenderUnit
	file ext:	h
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#ifndef RenderUnit_h__
#define RenderUnit_h__

class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;

class RenderUnit
{
public:
	RenderUnit(void);
	~RenderUnit();

	VertexBuffer* m_vb;
	IndexBuffer* m_ib;
	VertexShader* m_vs;
	PixelShader* m_ps;
};

#endif // RenderUnit_h__
