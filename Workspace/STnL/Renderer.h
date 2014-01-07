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

class BackBuffer;
class RenderUnit;
class PixelShader;
class DepthBuffer;

class Renderer
{
public:
	enum CullMode
	{
		CULL_MODE_CW,
		CULL_MODE_CCW,
		CULL_MODE_NONE
	};

public:
	Renderer(void);

	void SetRenderTarget(BackBuffer* renderTarget, DepthBuffer* depthBuffer);
	void AddRenderUnit(RenderUnit* renderUnit);
	void Render(void);

private:
	struct Triangle
	{
		unsigned short iV0;
		unsigned short iV1;
		unsigned short iV2;
		Vector3 lighting;
	};

	typedef std::vector<RenderUnit*> RenderUnitList;
	typedef std::vector<VertexShaderOutput> VsOutList;
	typedef std::vector<Triangle> TriangleList;

private:
	bool TrivialReject(Triangle& triangle, VsOutList& vsOuts);
	bool TrivialAccept(Triangle& triangle, VsOutList& vsOuts);
	bool RemoveBackface(Triangle& triangle, VsOutList& vsOuts, CullMode cullMode);
	void DrawLine(int x0, int y0, int x1, int y1, Color color);

	// 填充一条水平扫描线，线性插值两端点的顶点属性
	// 要求保证x0 < x1
	void FillSpan(float x0, float x1, int y, VertexShaderOutput& va0, VertexShaderOutput& va1, PixelShader& ps);

private:
	BackBuffer* m_renderTarget;
	DepthBuffer* m_depthBuffer;
	RenderUnitList m_renderUnitList;

	CullMode m_cullMode;
};

#endif // Renderer_h__
