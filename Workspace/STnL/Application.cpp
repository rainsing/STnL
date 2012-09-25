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

#include "DepthBuffer.h"
#include "TextOutput.h"
#include "Camera.h"
#include "InputCapturer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"
#include "RenderUnit.h"
#include "SceneObject.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "BackBuffer.h"
#include "Utilities.h"
#include "Types.h"

Application::Application( void )
{
	m_initialized = false;

	m_nTicks = 0;
	m_nTicksPerSecond = 0;

	m_backBuffer = NULL;
	m_renderer = NULL;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_inputCapturer = NULL;
	m_activeCamera = NULL;
	m_textOutput = NULL;
	m_depthBuffer = NULL;
}

Application::~Application()
{

}

void Application::Initialize( HWND hWnd, int windowWidth, int windowHeight )
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	m_nTicksPerSecond = freq.QuadPart;

	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	m_nTicks = counter.QuadPart;

	m_backBuffer = new BackBuffer(hWnd, windowWidth, windowHeight);
	m_renderer = new Renderer();
	m_meshManager = new MeshManager();
	m_textureManager = new TextureManager();
	m_inputCapturer = new InputCapturer();
	m_activeCamera = new Camera(float(windowWidth) / windowHeight);
	m_textOutput = new TextOutput(hWnd);
	m_depthBuffer = new DepthBuffer(windowWidth, windowHeight);
	
	Mesh* mesh = m_meshManager->LoadFromFile("..\\Media\\teapot.mesh");
	m_textureManager->LoadFromFile("..\\Media\\base.tga");
	m_textureManager->LoadFromFile("..\\Media\\normal.tga");

	SceneObject* object = new SceneObject(mesh, NULL);
	m_sceneObjectList.push_back(object);

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

	SAFE_DELETE(m_depthBuffer);
	SAFE_DELETE(m_textOutput);
	SAFE_DELETE(m_activeCamera);
	SAFE_DELETE(m_inputCapturer);
	SAFE_DELETE(m_textureManager);
	SAFE_DELETE(m_meshManager);
	SAFE_DELETE(m_renderer);
	SAFE_DELETE(m_backBuffer);
}

void Application::Update( void )
{
	if (!m_initialized)
	{
		return;
	}

	// 计算delta T
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	float dt = static_cast<float>((counter.QuadPart - m_nTicks) / double(m_nTicksPerSecond));
	m_nTicks = counter.QuadPart;

	// 显示FPS
	char str[256];
	sprintf_s(str, 256, "FPS: %.1f", 1.0f / dt);
	m_textOutput->Print(str, 5, 5);
	
	// 控制物体旋转
	// --begin--
	SceneObject* object = m_sceneObjectList[0];

	float rotationX = 0.0f;
	float rotationY = 0.0f;
	float rotationAmount = 90.0f * dt;

	if (m_inputCapturer->IsKeyDown(KC_UP_ARROW) && !m_inputCapturer->IsKeyDown(KC_DOWN_ARROW))
	{
		rotationX = rotationAmount;
	}
	else if (!m_inputCapturer->IsKeyDown(KC_UP_ARROW) && m_inputCapturer->IsKeyDown(KC_DOWN_ARROW))
	{
		rotationX = -rotationAmount;
	}

	if (m_inputCapturer->IsKeyDown(KC_LEFT_ARROW) && !m_inputCapturer->IsKeyDown(KC_RIGHT_ARROW))
	{
		rotationY = rotationAmount;
	}
	else if (!m_inputCapturer->IsKeyDown(KC_LEFT_ARROW) && m_inputCapturer->IsKeyDown(KC_RIGHT_ARROW))
	{
		rotationY = -rotationAmount;
	}

	if (rotationX != 0.0f || rotationY != 0.0f)
	{
		object->LocalRotate(rotationX, rotationY, 0.0f);
	}

	// 控制物体旋转
	// --end--

	// 控制摄像机移动
	// --begin--

	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float offsetZ = 0.0f;
	float offsetAmount = 10.0f * dt;

	if (m_inputCapturer->IsKeyDown(KC_A) && !m_inputCapturer->IsKeyDown(KC_D))
	{
		offsetX = -offsetAmount;
	}
	else if (!m_inputCapturer->IsKeyDown(KC_A) && m_inputCapturer->IsKeyDown(KC_D))
	{
		offsetX = offsetAmount;
	}

	if (m_inputCapturer->IsKeyDown(KC_F) && !m_inputCapturer->IsKeyDown(KC_R))
	{
		offsetY = -offsetAmount;
	}
	else if (!m_inputCapturer->IsKeyDown(KC_F) && m_inputCapturer->IsKeyDown(KC_R))
	{
		offsetY = offsetAmount;
	}

	if (m_inputCapturer->IsKeyDown(KC_W) && !m_inputCapturer->IsKeyDown(KC_S))
	{
		offsetZ = offsetAmount;
	}
	else if (!m_inputCapturer->IsKeyDown(KC_W) && m_inputCapturer->IsKeyDown(KC_S))
	{
		offsetZ = -offsetAmount;
	}

	if (offsetX != 0.0f || offsetY != 0.0f || offsetZ != 0.0f)
	{
		m_activeCamera->LocalMove(offsetX, offsetY, offsetZ);
	}

	// 控制摄像机移动
	// --end--
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

		myVS->worldMatrix = object->m_worldMatrix;
		MatrixTranspose(myVS->inverseWorldMatrix, myVS->worldMatrix);	// This only works when there is no translation or scaling!!!
		myVS->lightPosition = Vector3(30.0f, 15.0f, -15.0f);

		MatrixMultiply(myVS->worldViewProjMatrix, object->m_worldMatrix, m_activeCamera->GetViewMatrix());
		MatrixMultiply(myVS->worldViewProjMatrix, myVS->worldViewProjMatrix, m_activeCamera->GetProjMatrix());

		MyPixelShader* myPS = new MyPixelShader();
		renderUnit->m_ps = myPS;

		myPS->baseTexture = m_textureManager->GetTexture(0);
		myPS->normalTexture = m_textureManager->GetTexture(1);
		myPS->diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
		myPS->ambientColor = Vector3(0.05f, 0.05f, 0.05f);

		m_renderer->AddRenderUnit(renderUnit);
	}

	m_backBuffer->Clear();
	m_depthBuffer->Clear();

	m_renderer->SetRenderTarget(m_backBuffer, m_depthBuffer);
	m_renderer->Render();

	m_backBuffer->Present();
}
