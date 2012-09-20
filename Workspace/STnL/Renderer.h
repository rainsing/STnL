/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   17:29
	filename: 	e:\GitHub\STnL\Workspace\STnL\Renderer.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Renderer
	file ext:	h
	author:		Rainisng
	
	purpose:	实际负责执行渲染操作的对象
*********************************************************************/
#ifndef Renderer_h__
#define Renderer_h__

#include "VertexShader.h"

#include <vector>

class BackBuffer;
class RenderUnit;

class Renderer
{
public:
	Renderer(void);

	void SetRenderTarget(BackBuffer* renderTarget);
	void AddRenderUnit(RenderUnit* renderUnit);
	void Render(void);

private:
	struct Triangle
	{
		unsigned short iV0;
		unsigned short iV1;
		unsigned short iV2;
	};

	typedef std::vector<RenderUnit*> RenderUnitList;
	typedef std::vector<VertexShaderOutput> VsOutList;
	typedef std::vector<Triangle> TriangleList;

	enum CullMode
	{
		CULL_MODE_CW,
		CULL_MODE_CCW,
		CULL_MODE_NONE
	};

private:
	bool TrivialReject(Triangle& triangle, VsOutList& vsOuts);
	bool TrivialAccept(Triangle& triangle, VsOutList& vsOuts);
	bool RemoveBackface(Triangle& triangle, VsOutList& vsOuts, CullMode cullMode);
	void DrawLine(int x0, int y0, int x1, int y1, Color color);

private:
	BackBuffer* m_renderTarget;
	RenderUnitList m_renderUnitList;
};

#endif // Renderer_h__
