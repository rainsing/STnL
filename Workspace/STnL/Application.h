/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   10:55
	filename: 	e:\GitHub\STnL\Workspace\STnL\Application.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Application
	file ext:	h
	author:		Rainsing
	
	purpose:	�洢һЩӦ�ó����ȫ�ֶ���ʵ��Ӧ�ó����Init, Update,
				Render, Destory������WinMain����
*********************************************************************/
#ifndef Application_h__
#define Application_h__

class BackBuffer;
class Renderer;
class SceneObject;
class MeshManager;
class TextureManager;
class InputCapturer;
class Camera;
class TextOutput;
class DepthBuffer;
class MaterialManager;
class PointLight;

typedef std::vector<SceneObject*> SceneObjectList;

class Application
{
public:

	Application(void);
	~Application();

	void Initialize(HWND hWnd, int windowWidth, int windowHeight);
	void Destroy(void);
	void Update(void);
	void Render(void);
	
private:

	bool m_initialized;

	long long m_nTicks;
	long long m_nTicksPerSecond;

	BackBuffer* m_backBuffer;
	Renderer* m_renderer;
	SceneObjectList m_sceneObjectList;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	InputCapturer* m_inputCapturer;
	Camera* m_activeCamera;
	PointLight* m_activeLight;
	TextOutput* m_textOutput;
	DepthBuffer* m_depthBuffer;
	MaterialManager* m_materialManager;

	unsigned m_activeObjectIndex;
};

#endif // Application_h__
