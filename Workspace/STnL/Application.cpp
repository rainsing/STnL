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

#include "VertexShader.h"
#include "Mesh.h"
#include "RenderUnit.h"
#include "SceneObject.h"
#include "MeshManager.h"
#include "Renderer.h"
#include "BackBuffer.h"
#include "Utilities.h"
#include "Types.h"

Application::Application( void )
{
	m_initialized = false;
	m_backBuffer = NULL;
	m_renderer = NULL;
	m_meshManager = NULL;
}

Application::~Application()
{

}

void Application::Initialize( HWND hWnd, int windowWidth, int windowHeight )
{
	m_backBuffer = new BackBuffer(hWnd, windowWidth, windowHeight);
	m_renderer = new Renderer();
	m_meshManager = new MeshManager();
	
	Mesh* mesh = m_meshManager->LoadFromFile("cube");
	SceneObject* object = new SceneObject(mesh, NULL);
	m_sceneObjectList.push_back(object);

	// 把cube往远处挪一点
	object->m_worldMatrix.m43 = 5.0f;

	m_initialized = true;
}

void Application::Destroy( void )
{
	if (!m_initialized)
	{
		return;
	}

	for (unsigned i = 0; i < m_sceneObjectList.size(); i++)
	{
		delete m_sceneObjectList[i];
		m_sceneObjectList[i] = NULL;
	}

	if (m_meshManager)
	{
		delete m_meshManager;
		m_meshManager = NULL;
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

	SceneObject* object = m_sceneObjectList[0];

	// 让cube转一下儿
	object->LocalRotate(0.02f, 0.05f, 0.0f);
}

void Application::Render( void )
{
	if (!m_initialized)
	{
		return;
	}

	for (unsigned i = 0; i < m_sceneObjectList.size(); i++)
	{
		SceneObject* object = m_sceneObjectList[i];
		RenderUnit* renderUnit = new RenderUnit();

		renderUnit->m_vb = object->GetMesh()->GetVertexBuffer();
		renderUnit->m_ib = object->GetMesh()->GetIndexBuffer();

		MyVertexShader* myVS = new MyVertexShader();
		renderUnit->m_vs = myVS;

		Matrix4 projMatrix;
		MakeProjectionMatrix(projMatrix, 0.1f, 15.0f, 45.0f * 3.14f / 180.0f, 1.33f);
		MatrixMultiply(myVS->worldViewProjMatrix, object->m_worldMatrix, projMatrix);

		m_renderer->AddRenderUnit(renderUnit);
	}

	m_backBuffer->Clear();

	m_renderer->SetRenderTarget(m_backBuffer);
	m_renderer->Render();

	m_backBuffer->Present();
}
