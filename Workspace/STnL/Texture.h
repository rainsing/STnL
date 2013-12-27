/********************************************************************
	created:	2012/09/21
	created:	21:9:2012   16:27
	filename: 	e:\GitHub\STnL\Workspace\STnL\Texture.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Texture
	file ext:	h
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#ifndef Texture_h__
#define Texture_h__

#include "Types.h"
#include "Vector.h"

class Texture
{
	friend class TextureManager;

public:
	Texture(void);
	~Texture();
	void Initialize(int width, int height);

	Vector4 Sample(float u, float v);

private:
	int m_width;
	int m_height;
	unsigned char* m_colorData;
};

#endif // Texture_h__
