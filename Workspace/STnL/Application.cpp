/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   11:10
	filename: 	e:\GitHub\STnL\Workspace\STnL\Application.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Application
	file ext:	cpp
	author:		Rainsing
	
	purpose:	存储一些应用程序的全局对象，实现应用程序的Init, Update,
				Render, Destory函数由WinMain调用
*********************************************************************/
#include "stdafx.h"
#include "Application.h"

#include "Renderer.h"
#include "BackBuffer.h"
#include "Utilities.h"
#include "Types.h"

Application::Application( void )
{
	m_initialized = false;
	m_backBuffer = NULL;
	m_renderer = NULL;
}

Application::~Application()
{

}

void Application::Initialize( HWND hWnd, int windowWidth, int windowHeight )
{
	m_backBuffer = new BackBuffer(hWnd, windowWidth, windowHeight);
	m_renderer = new Renderer();

	m_initialized = true;
}

void Application::Destroy( void )
{
	if (!m_initialized)
	{
		return;
	}

	if (m_renderer)
	{
		delete m_renderer;
		m_renderer = NULL;
	}

	if (m_backBuffer)
	{
		delete m_backBuffer;
		m_backBuffer = NULL;
	}
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

	m_backBuffer->Clear();

	m_renderer->SetRenderTarget(m_backBuffer);
	m_renderer->Render();

	m_backBuffer->Present();
}
