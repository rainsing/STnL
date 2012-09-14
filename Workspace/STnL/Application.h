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
class BackBuffer;

class Application
{
public:

	Application(void);
	~Application();

	void Initialize(HWND hWnd, int windowWidth, int windowHeight);
	void Update(void);
	void Render(void);
	void Destroy(void);

private:

	bool m_initialized;

	BackBuffer* m_backBuffer;
};
