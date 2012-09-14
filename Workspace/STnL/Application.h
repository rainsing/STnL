/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   10:55
	filename: 	e:\GitHub\STnL\Workspace\STnL\Application.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Application
	file ext:	h
	author:		Rainsing
	
	purpose:	存储一些应用程序的全局对象，实现应用程序的Init, Update,
				Render, Destory函数由WinMain调用
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
