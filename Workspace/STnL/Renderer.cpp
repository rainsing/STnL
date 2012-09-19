/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   17:30
	filename: 	e:\GitHub\STnL\Workspace\STnL\Renderer.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Renderer
	file ext:	cpp
	author:		Rainsing
	
	purpose:	实际负责执行渲染操作的对象
*********************************************************************/
#include "stdafx.h"
#include "Renderer.h"

#include "RenderUnit.h"
#include "VertexShader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BackBuffer.h"
#include "Utilities.h"

Renderer::Renderer( void )
{
	m_renderTarget = NULL;
}

void Renderer::SetRenderTarget( BackBuffer* renderTarget )
{
	m_renderTarget = renderTarget;
}

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
		TriangleList triangles;

		// 对每个顶点执行VS
		for (unsigned j = 0; j < nVerts; j++)
		{
			vsOuts[j] = renderUnit->m_vs->Main((*renderUnit->m_vb)[j]);
		}

		// triangle setup (trivial-rejection, clipping)
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
			else if (TrivialAccept(tri, vsOuts))
			{
				triangles.push_back(tri);
			}
			else
			{
				// TODO: do clipping here
				triangles.push_back(tri);
			}
		}

		// perspective-divide
		for (unsigned j = 0; j < vsOuts.size(); j++)
		{
			Vector4& clipPos = vsOuts[j].clipPos;

			clipPos.x /= clipPos.w; 
			clipPos.y /= clipPos.w;
			clipPos.z /= clipPos.w;
		}
		
		// 光栅化每个三角形
		int halfRtWidth = m_renderTarget->GetWidth() >> 1;
		int halfRtHeight = m_renderTarget->GetHeight() >> 1;

		for (unsigned j = 0; j < triangles.size(); j++)
		{
			VertexShaderOutput& v0 = vsOuts[triangles[j].iV0];
			VertexShaderOutput& v1 = vsOuts[triangles[j].iV1];
			VertexShaderOutput& v2 = vsOuts[triangles[j].iV2];

			int x0 = int(( v0.clipPos.x + 1.0f) * halfRtWidth);
			int y0 = int((-v0.clipPos.y + 1.0f) * halfRtHeight);

			int x1 = int(( v1.clipPos.x + 1.0f) * halfRtWidth);
			int y1 = int((-v1.clipPos.y + 1.0f) * halfRtHeight);

			int x2 = int(( v2.clipPos.x + 1.0f) * halfRtWidth);
			int y2 = int((-v2.clipPos.y + 1.0f) * halfRtHeight);
			
			DrawLine(x0, y0, x1, y1, 0xFFFFFFFF);
			DrawLine(x0, y0, x2, y2, 0xFFFFFFFF);
			DrawLine(x1, y1, x2, y2, 0xFFFFFFFF);
		}

		delete renderUnit;
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
	if (v0.clipPos.x < -v0.clipPos.w && 
		v1.clipPos.x < -v1.clipPos.w && 
		v2.clipPos.x < -v2.clipPos.w)
	{
		return true;
	}

	// test against x = w plane
	if (v0.clipPos.x > v0.clipPos.w &&
		v1.clipPos.x > v1.clipPos.w &&
		v2.clipPos.x > v2.clipPos.w)
	{
		return true;
	}

	// test against y = -w plane
	if (v0.clipPos.y < -v0.clipPos.w && 
		v1.clipPos.y < -v1.clipPos.w && 
		v2.clipPos.y < -v2.clipPos.w)
	{
		return true;
	}

	// test against y = w plane
	if (v0.clipPos.y > v0.clipPos.w &&
		v1.clipPos.y > v1.clipPos.w &&
		v2.clipPos.y > v2.clipPos.w)
	{
		return true;
	}

	// test against z = 0 plane
	if (v0.clipPos.z < 0 && 
		v1.clipPos.z < 0 && 
		v2.clipPos.z < 0)
	{
		return true;
	}

	// test against z = w plane
	if (v0.clipPos.z > v0.clipPos.w &&
		v1.clipPos.z > v1.clipPos.w &&
		v2.clipPos.z > v2.clipPos.w)
	{
		return true;
	}

	// 避免渲染在摄像机后面的三角形
	// TODO: clipping to the Near plane to get rid of this
	if (v0.clipPos.z < 0.0f || 
		v1.clipPos.z < 0.0f || 
		v2.clipPos.z < 0.0f)
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
		v0.clipPos.x >= -v0.clipPos.w && v0.clipPos.x <= v0.clipPos.w &&
		v0.clipPos.y >= -v0.clipPos.w && v0.clipPos.y <= v0.clipPos.w &&
		v0.clipPos.z >= 0.0f && v0.clipPos.z <= v0.clipPos.w;

	bool v1InFrustum = 
		v1.clipPos.x >= -v1.clipPos.w && v1.clipPos.x <= v1.clipPos.w &&
		v1.clipPos.y >= -v1.clipPos.w && v1.clipPos.y <= v1.clipPos.w &&
		v1.clipPos.z >= 0.0f && v1.clipPos.z <= v1.clipPos.w;

	bool v2InFrustum = 
		v2.clipPos.x >= -v2.clipPos.w && v2.clipPos.x <= v2.clipPos.w &&
		v2.clipPos.y >= -v2.clipPos.w && v2.clipPos.y <= v2.clipPos.w &&
		v2.clipPos.z >= 0.0f && v2.clipPos.z <= v2.clipPos.w;

	return v0InFrustum && v1InFrustum && v2InFrustum;
}
