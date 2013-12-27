/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   13:12
	filename: 	e:\GitHub\STnL\Workspace\STnL\Material.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Material
	file ext:	h
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#ifndef Material_h__
#define Material_h__

enum VertexShaderId
{
	VS_FIXED_FUNCTION,
	VS_FIXED_FUNCTION_ALT_UV,
	VS_TANGENT_SPACE_LIGHTING,
	VS_TANGENT_SPACE_LIGHTING_SC2_UV
};

enum PixelShaderId
{
	PS_FIXED_FUNCTION,
	PS_NORMAL_MAP,
	PS_TOON_LIGHTING
};

class Material
{
public:
	int baseTextureId;
	int bumpTextureId;
	int vertexShaderId;
	int pixelShaderId;
	bool wireFrame;
};

typedef std::vector<Material*> MaterialList;

class MaterialManager
{
public:
	MaterialManager(void);
	~MaterialManager();

	Material* CreateMaterial(void);

private:
	MaterialList m_matList;
};

#endif // Material_h__
