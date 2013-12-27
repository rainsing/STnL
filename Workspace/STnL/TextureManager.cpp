/********************************************************************
	created:	2012/09/21
	created:	21:9:2012   16:21
	filename: 	e:\GitHub\STnL\Workspace\STnL\TextureManager.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	TextureManager
	file ext:	cpp
	author:		Rainsing
	
	purpose:	加载并管理纹理
*********************************************************************/
#include "stdafx.h"
#include "TextureManager.h"

#include "Texture.h"
#include "Utilities.h"

#include "IL/il.h"

TextureManager::TextureManager( void )
{
	// 初始化DevIL
	ilInit();
}

TextureManager::~TextureManager()
{
	for (unsigned i = 0; i < m_textureList.size(); i++)
	{
		SAFE_DELETE(m_textureList[i]);
	}
}

Texture* TextureManager::LoadFromFile(const char* fileName)
{
	ILuint imageId = 0;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	// DevIL这货几乎通吃所有的图片格式，太特么强大了！
	ilLoadImage(fileName);

	ILint width = ilGetInteger(IL_IMAGE_WIDTH);
	ILint height = ilGetInteger(IL_IMAGE_HEIGHT);

	Texture* texture = new Texture();
	texture->Initialize(width, height);

	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, texture->m_colorData);

	m_textureList.push_back(texture);

	ilBindImage(0);
	ilDeleteImage(imageId);
	
	return texture;
}
