/********************************************************************
	created:	2012/09/24
	created:	24:9:2012   18:13
	filename: 	e:\GitHub\STnL\Workspace\STnL\DepthBuffer.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	DepthBuffer
	file ext:	h
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#ifndef DepthBuffer_h__
#define DepthBuffer_h__

class DepthBuffer
{
public:
	DepthBuffer(int width, int height);
	~DepthBuffer();

	void Clear(void);
	bool TestDepth(int x, int y, float depth);

private:
	int m_width;
	int m_height;
	float* m_depthData;		// 32bit的深度缓冲没太大必要，可谁让C++没有“半精度”浮点呢……
};

#endif // DepthBuffer_h__
