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

class BackBuffer;

class Renderer
{
public:
	Renderer(void);

	void SetRenderTarget(BackBuffer* renderTarget);
	void Render(void);

	void DrawLine(int x0, int y0, int x1, int y1, Color color);

private:
	BackBuffer* m_renderTarget;
};

#endif // Renderer_h__