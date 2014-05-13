/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   17:30
	filename: 	e:\GitHub\STnL\Workspace\STnL\Renderer.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Renderer
	file ext:	cpp
	author:		Rainsing
	
	purpose:	This is where all core rendering operations happen.
*********************************************************************/
#include "stdafx.h"
#include "Renderer.h"

#include "Vertex.h"
#include "DepthBuffer.h"
#include "RenderUnit.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BackBuffer.h"
#include "Utilities.h"

#include <process.h>

Renderer::Renderer( void )
{
	m_renderTarget = NULL;
	m_cullMode = CULL_MODE_CCW;
    m_bExiting = false;

    m_spanFillingBacklogReadyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	for (int i = 0; i < m_numThreads; i++)
	{
        m_spanFillingWorkDoneEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

		m_spanFillingThreadStartArgs[i].renderer = this;
		m_spanFillingThreadStartArgs[i].threadIndex = i;

		m_spanFillingThreads[i] = (HANDLE)_beginthreadex(NULL, 0, SpanFillingThreadMain, (void*)(m_spanFillingThreadStartArgs + i), 0, NULL);
	}
}

Renderer::~Renderer()
{
    m_bExiting = true;

    // This is necessary for the threads to return from their main function.
    for (int i = 0; i < m_numThreads; i++)
    {
        SetEvent(m_spanFillingBacklogReadyEvent);
    }

    WaitForMultipleObjects(m_numThreads, m_spanFillingThreads, TRUE, INFINITE);

	for (int i = 0; i < m_numThreads; i++)
	{ 
		CloseHandle(m_spanFillingThreads[i]);
        CloseHandle(m_spanFillingWorkDoneEvents[i]);
	}

    CloseHandle(m_spanFillingBacklogReadyEvent);
}

void Renderer::SetRenderTarget( BackBuffer* renderTarget, DepthBuffer* depthBuffer )
{
	m_renderTarget = renderTarget;
	m_depthBuffer = depthBuffer;
}

// The classic Bresenham algorithm.
void Renderer::DrawLine( int x0, int y0, int x1, int y1, Color color )
{
	bool steep = abs(y1 - y0) > abs(x1 - x0);

	if (steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int deltaX = x1 - x0;
	int deltaY = abs(y1 - y0);
	int error = deltaX / 2;
	int yStep = y0 < y1 ? 1 : -1;
	int y = y0;

	for (int x = x0; x <= x1; x++)
	{
		steep ? m_renderTarget->SetPixel(y, x, color) : m_renderTarget->SetPixel(x, y, color);
		error -= deltaY;
		if (error < 0)
		{
			y += yStep;
			error += deltaX;
		}
	}
}

void Renderer::Render( void )
{
	for (unsigned i = 0; i < m_renderUnitList.size(); i++)
	{
		RenderUnit* renderUnit = m_renderUnitList[i];

		unsigned nVerts = renderUnit->m_vb->Size();
		unsigned nTriangles = renderUnit->m_ib->Size() / 3;

		VsOutList vsOuts(nVerts);
		TriangleList triangles;		// triangles that are spared by trivial rejection and backface removal

		// Execute vertex shader for each vertex.
		for (unsigned j = 0; j < nVerts; j++)
		{
			vsOuts[j] = renderUnit->m_vs->Main((*renderUnit->m_vb)[j]);
		}

		// triangle setup
		for (unsigned j = 0; j < nTriangles; j++)
		{
			Triangle tri;
			tri.iV0 = (*renderUnit->m_ib)[3 * j + 0];
			tri.iV1 = (*renderUnit->m_ib)[3 * j + 1];
			tri.iV2 = (*renderUnit->m_ib)[3 * j + 2];

			if (TrivialReject(tri, vsOuts))
			{
				continue;
			}
			else if (RemoveBackface(tri, vsOuts, m_cullMode))
			{
				continue;
			}
			/*else if (TrivialAccept(tri, vsOuts))
			{
				triangles.push_back(tri);
			}*/
			else
			{
				triangles.push_back(tri);
			}
		}

		int halfRtWidth = m_renderTarget->GetWidth() >> 1;
		int halfRtHeight = m_renderTarget->GetHeight() >> 1;

		for (unsigned j = 0; j < vsOuts.size(); j++)
		{
			Vector4& position = vsOuts[j].position;

			// perspective-divide
			position.x /= position.w; 
			position.y /= position.w;
			position.z /= position.w;

			// to screen space
			position.x = ( position.x + 1.0f) * halfRtWidth;
			position.y = (-position.y + 1.0f) * halfRtHeight;
		}
		
		// rasterization
		for (unsigned j = 0; j < triangles.size(); j++)
		{
			VertexShaderOutput& v0 = vsOuts[triangles[j].iV0];
			VertexShaderOutput& v1 = vsOuts[triangles[j].iV1];
			VertexShaderOutput& v2 = vsOuts[triangles[j].iV2];

			if (renderUnit->m_wireFrame)	// fill mode: wireframe
			{
				int x0 = int(v0.position.x);
				int y0 = int(v0.position.y);
				int x1 = int(v1.position.x);
				int y1 = int(v1.position.y);
				int x2 = int(v2.position.x);
				int y2 = int(v2.position.y);

				DrawLine(x0, y0, x1, y1, 0xFFFFFFFF);
				DrawLine(x0, y0, x2, y2, 0xFFFFFFFF);
				DrawLine(x1, y1, x2, y2, 0xFFFFFFFF);
			}
			else	// fill mode: solid
			{
				VertexShaderOutput* sv;
				VertexShaderOutput* mv;
				VertexShaderOutput* ev;
				
				// sort the 3 vertices of a triangle by their Y position
				if (v0.position.y < v1.position.y)
				{
					sv = &v0;
					ev = &v1;
				}
				else
				{
					sv = &v1;
					ev = &v0;
				}

				if (v2.position.y < sv->position.y)
				{
					mv = sv;
					sv = &v2;
				}
				else if (v2.position.y > ev->position.y)
				{
					mv = ev;
					ev = &v2;
				}
				else
				{
					mv = &v2;
				}

				// scan line algorithm
				// --begin--

				int minY = int(sv->position.y);
				int maxY = int(ev->position.y);
				int midY = int(mv->position.y);

				float esY = ev->position.y - sv->position.y;
				float msY = mv->position.y - sv->position.y;
				float emY = ev->position.y - mv->position.y;

				float subPixelYOffsetS = sv->position.y - int(sv->position.y);
				float subPixelYOffsetM = mv->position.y - int(mv->position.y);

				float dx1 = (ev->position.x - sv->position.x) / esY;
				float dx2 = (mv->position.x - sv->position.x) / msY;	
				float dx2Alt = (ev->position.x - mv->position.x) / emY;

				float x1 = sv->position.x;
				float x2 = sv->position.x;

				for (int y = minY; y < maxY; y++)
				{
					if (y == midY)
					{
						dx2 = dx2Alt;
						x2 = mv->position.x;
					}

					float fY = y < midY ? y + subPixelYOffsetS : y + subPixelYOffsetM;

					VertexShaderOutput& va1 = Lerp(*sv, *ev, (ev->position.y - fY) / esY);

					VertexShaderOutput& va2 = y < midY
						? Lerp(*sv, *mv, (mv->position.y - fY) / msY)
						: Lerp(*mv, *ev, (ev->position.y - fY) / emY);

					if (x1 < x2)
					{
                        DispatchSpanFillingWork(x1, x2, y, va1, va2, *renderUnit->m_ps);
					}
					else
					{
                        DispatchSpanFillingWork(x2, x1, y, va2, va1, *renderUnit->m_ps);
					}

					x1 += dx1;
					x2 += dx2;
				}

				// scan line algorithm
				// --end--
			}
		}
	}

    // Tell the worker threads to start doing their job.
    // The loop is nessesary as each time m_spanFillingBacklogReadyEvent is signaled 
    // only one thread will be released from waiting.
    for (int i = 0; i < m_numThreads; i++)
    {
        SetEvent(m_spanFillingBacklogReadyEvent);
    }

    // Wait here until all the worker threads are done with their job.
    WaitForMultipleObjects(m_numThreads, m_spanFillingWorkDoneEvents, TRUE, INFINITE);

    for (int i = 0; i < m_numThreads; i++)
    {
        m_spanFillingBacklogs[i].clear();
    }

    for (unsigned i = 0; i < m_renderUnitList.size(); i++)
    {
        delete m_renderUnitList[i];
    }

	m_renderUnitList.clear();
}

void Renderer::AddRenderUnit( RenderUnit* renderUnit )
{
	m_renderUnitList.push_back(renderUnit);
}

bool Renderer::TrivialReject( Triangle& triangle, VsOutList& vsOuts )
{
	VertexShaderOutput& v0 = vsOuts[triangle.iV0];
	VertexShaderOutput& v1 = vsOuts[triangle.iV1];
	VertexShaderOutput& v2 = vsOuts[triangle.iV2];

	// test against x = -w plane
	if (v0.position.x < -v0.position.w && 
		v1.position.x < -v1.position.w && 
		v2.position.x < -v2.position.w)
	{
		return true;
	}

	// test against x = w plane
	if (v0.position.x > v0.position.w &&
		v1.position.x > v1.position.w &&
		v2.position.x > v2.position.w)
	{
		return true;
	}

	// test against y = -w plane
	if (v0.position.y < -v0.position.w && 
		v1.position.y < -v1.position.w && 
		v2.position.y < -v2.position.w)
	{
		return true;
	}

	// test against y = w plane
	if (v0.position.y > v0.position.w &&
		v1.position.y > v1.position.w &&
		v2.position.y > v2.position.w)
	{
		return true;
	}

	// test against z = w plane
	if (v0.position.z > v0.position.w &&
		v1.position.z > v1.position.w &&
		v2.position.z > v2.position.w)
	{
		return true;
	}

	// test against z = 0 plane
	// No clipping here. Any triangle that is partially behind the camera is completely discarded.
	if (v0.position.z < 0.0f || 
		v1.position.z < 0.0f || 
		v2.position.z < 0.0f)
	{
		return true;
	}

	return false;
}

bool Renderer::TrivialAccept( Triangle& triangle, VsOutList& vsOuts )
{
	VertexShaderOutput& v0 = vsOuts[triangle.iV0];
	VertexShaderOutput& v1 = vsOuts[triangle.iV1];
	VertexShaderOutput& v2 = vsOuts[triangle.iV2];

	bool v0InFrustum = 
		v0.position.x >= -v0.position.w && v0.position.x <= v0.position.w &&
		v0.position.y >= -v0.position.w && v0.position.y <= v0.position.w &&
		v0.position.z >= 0.0f && v0.position.z <= v0.position.w;

	bool v1InFrustum = 
		v1.position.x >= -v1.position.w && v1.position.x <= v1.position.w &&
		v1.position.y >= -v1.position.w && v1.position.y <= v1.position.w &&
		v1.position.z >= 0.0f && v1.position.z <= v1.position.w;

	bool v2InFrustum = 
		v2.position.x >= -v2.position.w && v2.position.x <= v2.position.w &&
		v2.position.y >= -v2.position.w && v2.position.y <= v2.position.w &&
		v2.position.z >= 0.0f && v2.position.z <= v2.position.w;

	return v0InFrustum && v1InFrustum && v2InFrustum;
}

bool Renderer::RemoveBackface( Triangle& triangle, VsOutList& vsOuts, CullMode cullMode )
{
	if (cullMode == CULL_MODE_NONE)
	{
		return false;
	}

	VertexShaderOutput& v0 = vsOuts[triangle.iV0];
	VertexShaderOutput& v1 = vsOuts[triangle.iV1];
	VertexShaderOutput& v2 = vsOuts[triangle.iV2];

	float w0R = 1.0f / v0.position.w;
	float w1R = 1.0f / v1.position.w;
	float w2R = 1.0f / v2.position.w;

	Vector3 e1;
	e1.x = v1.position.x * w1R - v0.position.x * w0R;
	e1.y = v1.position.y * w1R - v0.position.y * w0R;
	e1.z = v1.position.z * w1R - v0.position.z * w0R;

	Vector3 e2;
	e2.x = v2.position.x * w2R - v0.position.x * w0R;
	e2.y = v2.position.y * w2R - v0.position.y * w0R;
	e2.z = v2.position.z * w2R - v0.position.z * w0R;

	Vector3 faceNormal = e1.Cross(e2);

	/*if (faceNormal.Equal(Vector3::ZERO, 0.0001f))
	{
		return false;
	}*/

	if (cullMode == CULL_MODE_CCW)
	{
		return faceNormal.z > 0.0f;
	}
	else
	{
		return faceNormal.z < 0.0f;
	}
}

void Renderer::FillSpan( float x0, float x1, int y, VertexShaderOutput& va0, VertexShaderOutput& va1, PixelShader& ps )
{
	int startX = Float2Int(x0);
	int endX = Float2Int(x1);
	float lenR = 1.0f / (x1 - x0);
	float xt = x0;

	VertexShaderOutput va;
	for (int x = startX; x < endX; x++)
	{
		xt = float(x + 1);
		Lerp(va, va0, va1, (x1 - xt) * lenR);

		// early z-test (before executing pixel shader)
		if (!m_depthBuffer->TestDepth(x, y, va.position.z))
		{
			continue;
		}
		
		// execute the pixel shader
		Vector4& color = ps.Main(va);

		int r = Float2Int(color.x * 255.0f);
		int g = Float2Int(color.y * 255.0f);
		int b = Float2Int(color.z * 255.0f);
		m_renderTarget->SetPixel(x, y, COLOR_RGB(r, g, b));
	}
}

void Renderer::DispatchSpanFillingWork( float x0, float x1, int y, VertexShaderOutput& va0, VertexShaderOutput& va1, PixelShader& ps )
{
    if (y < 0 || y >= m_renderTarget->GetHeight())
    {
        return;
    }

    int h = m_renderTarget->GetHeight() / m_numThreads;
    SpanFillingBacklog* workQueue = m_spanFillingBacklogs + y / h;

    // VS2012's vector::emplace_back() implementation doesn't support more than 5 arguments.
    // That's why we have a Vector2 here...
    workQueue->emplace_back(Vector2(x0, x1), y, va0, va1, ps);  
}

unsigned __stdcall Renderer::SpanFillingThreadMain(void* startArgs)
{
	SpanFillingThreadStartArgs& args = *((SpanFillingThreadStartArgs*)startArgs);
    Renderer* renderer = args.renderer;
    int threadIndex = args.threadIndex;
    SpanFillingBacklog& backlog = renderer->m_spanFillingBacklogs[threadIndex];

    while (!renderer->m_bExiting)
    {
        // wait for the main thread to feed us work
        WaitForSingleObject(renderer->m_spanFillingBacklogReadyEvent, INFINITE);

        // do the work
        for (unsigned i = 0; i < backlog.size(); i++)
        {
            SpanFillingWork& work = backlog[i];
            renderer->FillSpan(work.x0, work.x1, work.y, work.va0, work.va1, *(work.pixelShader));
        }

        // tell the main thread that we've finished our work
        SetEvent(renderer->m_spanFillingWorkDoneEvents[threadIndex]);
    }

	return 0;
}
