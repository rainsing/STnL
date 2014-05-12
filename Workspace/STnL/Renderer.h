/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   17:29
	filename: 	e:\GitHub\STnL\Workspace\STnL\Renderer.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Renderer
	file ext:	h
	author:		Rainisng
	
	purpose:	This is where all core rendering operations happen.
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
	~Renderer();

	void SetRenderTarget(BackBuffer* renderTarget, DepthBuffer* depthBuffer);
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

private:
	bool TrivialReject(Triangle& triangle, VsOutList& vsOuts);
	bool TrivialAccept(Triangle& triangle, VsOutList& vsOuts);
	bool RemoveBackface(Triangle& triangle, VsOutList& vsOuts, CullMode cullMode);
	void DrawLine(int x0, int y0, int x1, int y1, Color color);

	// Fill a horizontal scan line. The pixel shader is executed for each pixel along the scan line.
	// It's required that x0 < x1.
	void FillSpan(float x0, float x1, int y, VertexShaderOutput& va0, VertexShaderOutput& va1, PixelShader& ps);

    void DispatchSpanFill(float x0, float x1, int y, VertexShaderOutput& va0, VertexShaderOutput& va1, PixelShader& ps);

private:
	BackBuffer* m_renderTarget;
	DepthBuffer* m_depthBuffer;
	RenderUnitList m_renderUnitList;

	CullMode m_cullMode;

	static const int m_numThreads = 4;
	HANDLE m_threadHandles[m_numThreads];

    struct WorkItem
    {
        float x0;
        float x1;
        int y;
        VertexShaderOutput va0;
        VertexShaderOutput va1;
        PixelShader* pixelShader;

        WorkItem(Vector2 _x01, int _y, VertexShaderOutput& _va0, VertexShaderOutput& _va1, PixelShader& _ps)
        {
            x0 = _x01.x;
            x1 = _x01.y;
            y = _y;
            va0 = _va0;
            va1 = _va1;
            pixelShader = &_ps;
        }
    };

	typedef std::vector<WorkItem> WorkQueue;
	WorkQueue m_threadWorkQueues[m_numThreads];

    HANDLE m_jobStartEvent;
    HANDLE m_jobDoneEvents[m_numThreads];

	struct ThreadStartParamters
	{
		Renderer* renderer;
		int threadIndex;
	};
	ThreadStartParamters m_threadStartParameters[m_numThreads];

    bool m_bExiting;

	static unsigned __stdcall ThreadFunction( void* data );
};

#endif // Renderer_h__
