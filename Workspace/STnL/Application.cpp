/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   11:10
	filename: 	e:\GitHub\STnL\Workspace\STnL\Application.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Application
	file ext:	cpp
	author:		Rainsing
	
	purpose:	�洢һЩӦ�ó����ȫ�ֶ���ʵ��Ӧ�ó����Init, Update,
				Render, Destory������WinMain����
*********************************************************************/
#include "stdafx.h"
#include "Application.h"

#include "BackBuffer.h"
#include "Types.h"

Application::Application( void )
{
	m_initialized = false;
	m_backBuffer = NULL;
}

Application::~Application()
{

}

void Application::Initialize( HWND hWnd, int windowWidth, int windowHeight )
{
	m_backBuffer = new BackBuffer(hWnd, windowWidth, windowHeight);

	m_initialized = true;
}

void Application::Update( void )
{
	if (!m_initialized)
	{
		return;
	}
}

void Application::Render( void )
{
	if (!m_initialized)
	{
		return;
	}

	m_backBuffer->Clear(COLOR_RGB(0, 0, 0));

	m_backBuffer->SetPixel(320, 240, COLOR_RGB(255, 255, 255));

	m_backBuffer->Present();
}

void Application::Destroy( void )
{
	if (!m_initialized)
	{
		return;
	}

	if (m_backBuffer)
	{
		delete m_backBuffer;
		m_backBuffer = NULL;
	}
}
