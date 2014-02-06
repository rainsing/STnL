/********************************************************************
	created:	2012/09/21
	created:	21:9:2012   16:21
	filename: 	e:\GitHub\STnL\Workspace\STnL\TextureManager.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	TextureManager
	file ext:	h
	author:		Rainsing
	
	purpose:	Load textures form file and maintain a list of loaded texutres.
*********************************************************************/
#ifndef TextureManager_h__
#define TextureManager_h__

class Texture;

typedef std::vector<Texture*> TextureList;

class TextureManager
{
public:
	TextureManager(void);
	~TextureManager();

    // load an image file into a texture object,
    // and return an integer ID of the texture.
	int LoadFromFile(const char* fileName);

    // Retrieve a texture object by its ID.
	inline Texture* GetTexture(int index);

private:
	TextureList m_textureList;
};

Texture* TextureManager::GetTexture( int index )
{
	if (index == -1)
	{
		return NULL;
	}

	return m_textureList[index];
}

#endif // TextureManager_h__
