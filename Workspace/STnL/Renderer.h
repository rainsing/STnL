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

    struct SpanFillingThreadStartArgs
    {
        Renderer* renderer;
        int threadIndex;
    };

    // This struct holds all the necessary data to fill a span.
    struct SpanFillingWork
    {
        float x0;
        float x1;
        int y;
        VertexShaderOutput va0;
        VertexShaderOutput va1;
        PixelShader* pixelShader;

        SpanFillingWork(Vector2& _x01, int _y, VertexShaderOutput& _va0, VertexShaderOutput& _va1, PixelShader& _ps)
        {
            x0 = _x01.x;
            x1 = _x01.y;
            y = _y;
            va0 = _va0;
            va1 = _va1;
            pixelShader = &_ps;
        }
    };

    typedef std::vector<SpanFillingWork> SpanFillingBacklog;

private:
	bool TrivialReject(Triangle& triangle, VsOutList& vsOuts);
	bool TrivialAccept(Triangle& triangle, VsOutList& vsOuts);
	bool RemoveBackface(Triangle& triangle, VsOutList& vsOuts, CullMode cullMode);
	void DrawLine(int x0, int y0, int x1, int y1, Color color);

	// Fill a horizontal scan line. The pixel shader is executed for each pixel along the scan line.
	// It's required that x0 < x1.
	void FillSpan(float x0, float x1, int y, VertexShaderOutput& va0, VertexShaderOutput& va1, PixelShader& ps);

    // The strategy we devised for multi-threaded rendering is like this:
    // We evenly devide the screen into a few portions along the vertical direction, 
    // then for each of the portions we create a thread that is only responsible for the rendering of that particular portion.
    // In this way we guaranteed that the multiple threads will never vie with each other for the exclusive access
    // to the shared back buffer / depth buffer.

    // thread main function
    static unsigned __stdcall SpanFillingThreadMain(void* startArgs);

    // This function dispatches the work of filling a segmented scan line to the proper worker thread (acoording to the y of the scan line).
    void DispatchSpanFillingWork(float x0, float x1, int y, VertexShaderOutput& va0, VertexShaderOutput& va1, PixelShader& ps);

private:
	BackBuffer* m_renderTarget;
	DepthBuffer* m_depthBuffer;
	RenderUnitList m_renderUnitList;

	CullMode m_cullMode;

	static const int m_numThreads = 4;

	HANDLE m_spanFillingThreads[m_numThreads];
    SpanFillingThreadStartArgs m_spanFillingThreadStartArgs[m_numThreads];
	SpanFillingBacklog m_spanFillingBacklogs[m_numThreads];

    // These events are auto-reseting and initially nonsignaled.
    HANDLE m_spanFillingBacklogReadyEvent;
    HANDLE m_spanFillingWorkDoneEvents[m_numThreads];

    bool m_bExiting;
};

#endif // Renderer_h__
