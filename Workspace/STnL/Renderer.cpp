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
			else if (RemoveBackface(tri, vsOuts, CULL_MODE_CCW))
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

		int halfRtWidth = m_renderTarget->GetWidth() >> 1;
		int halfRtHeight = m_renderTarget->GetHeight() >> 1;

		for (unsigned j = 0; j < vsOuts.size(); j++)
		{
			Vector4& position = vsOuts[j].position;

			// perspective-divide
			position.x /= position.w; 
			position.y /= position.w;
			position.z /= position.w;

			// 转化到屏幕坐标
			position.x = ( position.x + 1.0f) * halfRtWidth;
			position.y = (-position.y + 1.0f) * halfRtHeight;
		}
		
		// 光栅化每个三角形
		for (unsigned j = 0; j < triangles.size(); j++)
		{
			VertexShaderOutput& v0 = vsOuts[triangles[j].iV0];
			VertexShaderOutput& v1 = vsOuts[triangles[j].iV1];
			VertexShaderOutput& v2 = vsOuts[triangles[j].iV2];

			bool wireFrameMode = false;

			if (wireFrameMode)	// fill mode: wireframe
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
				
				// 按Y值给三个顶点排序
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

				// 扫描线算法
				int minY = int(sv->position.y);
				int maxY = int(ev->position.y);
				int midY = int(mv->position.y);
				float x1 = sv->position.x;
				float x2 = sv->position.x;
				float dx1 = (ev->position.x - sv->position.x) / (ev->position.y - sv->position.y);
				float dx2 = (mv->position.x - sv->position.x) / (mv->position.y - sv->position.y);	
				float dx2Alt = (ev->position.x - mv->position.x) / (ev->position.y - mv->position.y);

				for (int y = minY; y < maxY; y++)
				{
					if (y == midY)
					{
						x2 = mv->position.x;
						dx2 = dx2Alt;
					}

					DrawLine(int(x1), y, int(x2), y, COLOR_RGB(sv->position.x / 640.0f * 255, 0, 0));

					x1 += dx1;
					x2 += dx2;
				}
			}
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

	// test against z = 0 plane
	if (v0.position.z < 0 && 
		v1.position.z < 0 && 
		v2.position.z < 0)
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

	// 避免渲染在摄像机后面的三角形
	// TODO: clipping to the Near plane to get rid of this
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

	Vector3 e1;
	e1.x = v1.position.x / v1.position.w - v0.position.x / v0.position.w;
	e1.y = v1.position.y / v1.position.w - v0.position.y / v0.position.w;
	e1.z = v1.position.z / v1.position.w - v0.position.z / v0.position.w;

	Vector3 e2;
	e2.x = v2.position.x / v2.position.w - v0.position.x / v0.position.w;
	e2.y = v2.position.y / v2.position.w - v0.position.y / v0.position.w;
	e2.z = v2.position.z / v2.position.w - v0.position.z / v0.position.w;

	Vector3 faceNormal = e1.Cross(e2);

	// 顺带在这儿移除所有的退化三角形
	if (faceNormal.Equal(Vector3::ZERO, 0.0001f))
	{
		return true;
	}

	if (cullMode == CULL_MODE_CCW)
	{
		return faceNormal.z > 0;
	}
	else
	{
		return faceNormal.z < 0;
	}
}
