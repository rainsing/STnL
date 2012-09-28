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

#include "PointLight.h"
#include "Material.h"
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
	m_activeLight = NULL;
	m_textOutput = NULL;
	m_depthBuffer = NULL;
	m_materialManager = NULL;
}

Application::~Application()
{
	if (m_initialized)
	{
		Destroy();
	}
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
	m_textOutput = new TextOutput(hWnd);
	m_depthBuffer = new DepthBuffer(windowWidth, windowHeight);
	m_materialManager = new MaterialManager();

	m_textureManager->LoadFromFile("..\\Media\\base.tga");
	m_textureManager->LoadFromFile("..\\Media\\normal.tga");
	m_textureManager->LoadFromFile("..\\Media\\marine_diffuse.dds");

	// ��ʼ������
	// --begin--
	SceneObject* object;
	Mesh* mesh;
	Material* mat;

	// ����1�����
	mesh = m_meshManager->LoadFromFile("..\\Media\\teapot.mesh");

	mat = m_materialManager->CreateMaterial();
	mat->baseTextureId = 0;
	mat->bumpTextureId = 1;
	mat->vertexShaderId = VS_TANGENT_SPACE_LIGHTING;
	mat->pixelShaderId = PS_NORMAL_MAP;
	mat->wireFrame = false;

	object = new SceneObject(mesh, mat);
	m_sceneObjectList.push_back(object);

	// ����1.5���߿���
	//mesh = m_meshManager->LoadFromFile("..\\Media\\teapot.mesh");

	mat = m_materialManager->CreateMaterial();
	mat->baseTextureId = 0;
	mat->bumpTextureId = 1;
	mat->vertexShaderId = VS_TANGENT_SPACE_LIGHTING;
	mat->pixelShaderId = PS_NORMAL_MAP;
	mat->wireFrame = true;

	object = new SceneObject(mesh, mat);
	m_sceneObjectList.push_back(object);

	// ����2����ǹ����
	mesh = m_meshManager->LoadFromFile("..\\Media\\marine.mesh");

	mat = m_materialManager->CreateMaterial();
	mat->baseTextureId = 2;
	mat->bumpTextureId = -1;
	mat->vertexShaderId = VS_TANGENT_SPACE_LIGHTING_SC2_UV;
	mat->pixelShaderId = PS_TOON_LIGHTING;
	mat->wireFrame = false;

	object = new SceneObject(mesh, mat);
	m_sceneObjectList.push_back(object);

	m_activeObjectIndex = 0;
	m_sceneObjectList[m_activeObjectIndex]->Hide(false);

	// �����
	m_activeCamera = new Camera(
		Vector3(0.0f, 5.0f, -40.0f), 
		Vector3(0.0f, 5.0f, 0.0f), 
		0.1f, 
		100.0f, 
		45.0f, 
		float(windowWidth) / windowHeight
	);

	// ��Դ
	m_activeLight = new PointLight();
	m_activeLight->position = Vector3(30.0f, 15.0f, -15.0f);
	m_activeLight->diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	m_activeLight->ambientColor = Vector3(0.1f, 0.1f, 0.1f);

	// ��ʼ������
	// --end--

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

	SAFE_DELETE(m_activeLight);
	SAFE_DELETE(m_activeCamera);
	SAFE_DELETE(m_materialManager);
	SAFE_DELETE(m_depthBuffer);
	SAFE_DELETE(m_textOutput);
	SAFE_DELETE(m_inputCapturer);
	SAFE_DELETE(m_textureManager);
	SAFE_DELETE(m_meshManager);
	SAFE_DELETE(m_renderer);
	SAFE_DELETE(m_backBuffer);

	m_initialized = false;
}

void Application::Update( void )
{
	if (!m_initialized)
	{
		return;
	}

	// ����delta T
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	float dt = static_cast<float>((counter.QuadPart - m_nTicks) / double(m_nTicksPerSecond));
	m_nTicks = counter.QuadPart;

	// ��ʾFPS
	char str[256];
	sprintf_s(str, 256, "FPS: %.1f", 1.0f / dt);
	m_textOutput->Print(str, 5, 5);

	// �л�shading mode
	if (m_inputCapturer->IsKeyPressed(KC_T))
	{
		int shadeMode = m_renderer->GetShadeMode();

		if (shadeMode == Renderer::SHADING_MODE_MAX - 1)
		{
			shadeMode = Renderer::SHADING_MODE_START;

		}

		shadeMode++;
		m_renderer->SetShadeMode(static_cast<Renderer::ShadeMode>(shadeMode));
	}

	// �л���ǰ����
	if (m_inputCapturer->IsKeyPressed(KC_C))
	{
		m_sceneObjectList[m_activeObjectIndex]->Hide(true);

		if (m_activeObjectIndex < m_sceneObjectList.size() - 1)
		{
			m_activeObjectIndex++;
		}
		else
		{
			m_activeObjectIndex = 0;
		}

		m_sceneObjectList[m_activeObjectIndex]->Hide(false);
	}

	SceneObject* object = m_sceneObjectList[m_activeObjectIndex];
	
	// ����������ת
	// --begin--

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

	// ����������ת
	// --end--

	// ����������ƶ�
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

	if (m_inputCapturer->IsKeyDown(KC_Q) && !m_inputCapturer->IsKeyDown(KC_E))
	{
		offsetY = -offsetAmount;
	}
	else if (!m_inputCapturer->IsKeyDown(KC_Q) && m_inputCapturer->IsKeyDown(KC_E))
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

	// ����������ƶ�
	// --end--

	// �����������ת
	// --begin--

	if (m_inputCapturer->IsLeftBtnDown())
	{
		int dx, dy;
		m_inputCapturer->GetMouseMovement(dx, dy);

		if (dx != 0 || dy != 0)
		{
			m_activeCamera->LocalRotate(dy * 0.5f, dx * 0.5f);
		}
	}

	// �����������ת
	// --end--

	// ���ó���
	if (m_inputCapturer->IsKeyPressed(KC_R))
	{
		object->ResetRotation();
		m_activeCamera->Reset();
	}

	m_inputCapturer->ClearMouseMovement();
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
		Material* material = object->GetMaterial();

		if (object->IsHidden())
		{
			continue;
		}

		RenderUnit* renderUnit = new RenderUnit();

		renderUnit->m_vb = object->GetMesh()->GetVertexBuffer();
		renderUnit->m_ib = object->GetMesh()->GetIndexBuffer();
		renderUnit->m_wireFrame = material->wireFrame;

		int vertexShaderId = m_renderer->GetShadeMode() == Renderer::SHADING_MODE_PHONG 
			? material->vertexShaderId 
			: (material->vertexShaderId == VS_TANGENT_SPACE_LIGHTING_SC2_UV ? VS_FIXED_FUNCTION_ALT_UV : VS_FIXED_FUNCTION);

		int pixelShaderId = m_renderer->GetShadeMode() == Renderer::SHADING_MODE_PHONG 
			? material->pixelShaderId 
			: PS_FIXED_FUNCTION;

		switch (vertexShaderId)
		{
		case VS_FIXED_FUNCTION:
			{
				VsFixedFunction* myVS = new VsFixedFunction();
				renderUnit->m_vs = myVS;

				Matrix4 worldMatrix;
				object->GetWorldMatrix(worldMatrix);
				MatrixTranspose(myVS->inverseWorldMatrix, worldMatrix);	// This only works when there is no translation 
																		// or scaling!!!
				MatrixMultiply(myVS->worldViewProjMatrix, worldMatrix, m_activeCamera->GetViewMatrix());
				MatrixMultiply(myVS->worldViewProjMatrix, myVS->worldViewProjMatrix, m_activeCamera->GetProjMatrix());

				myVS->lightPosition = m_activeLight->position;

				myVS->ambientColor = m_activeLight->ambientColor;
				myVS->diffuseColor = m_activeLight->diffuseColor;
			}
			break;
		case VS_FIXED_FUNCTION_ALT_UV:
			{
				VsFixedFunctionAltUv* myVS = new VsFixedFunctionAltUv();
				renderUnit->m_vs = myVS;

				Matrix4 worldMatrix;
				object->GetWorldMatrix(worldMatrix);
				MatrixTranspose(myVS->inverseWorldMatrix, worldMatrix);	// This only works when there is no translation 
				// or scaling!!!
				MatrixMultiply(myVS->worldViewProjMatrix, worldMatrix, m_activeCamera->GetViewMatrix());
				MatrixMultiply(myVS->worldViewProjMatrix, myVS->worldViewProjMatrix, m_activeCamera->GetProjMatrix());

				myVS->lightPosition = m_activeLight->position;

				myVS->ambientColor = m_activeLight->ambientColor;
				myVS->diffuseColor = m_activeLight->diffuseColor;
			}
			break;
		case VS_TANGENT_SPACE_LIGHTING:
			{
				VsTangentSpaceLighting* myVS = new VsTangentSpaceLighting();
				renderUnit->m_vs = myVS;

				object->GetWorldMatrix(myVS->worldMatrix);
				MatrixTranspose(myVS->inverseWorldMatrix, myVS->worldMatrix);	// This only works when there is no translation 
																				// or scaling!!!
				MatrixMultiply(myVS->worldViewProjMatrix, myVS->worldMatrix, m_activeCamera->GetViewMatrix());
				MatrixMultiply(myVS->worldViewProjMatrix, myVS->worldViewProjMatrix, m_activeCamera->GetProjMatrix());

				myVS->lightPosition = m_activeLight->position;
			}
			break;

		case VS_TANGENT_SPACE_LIGHTING_SC2_UV:
			{
				VsTangentSpaceLightingSc2Uv* myVS = new VsTangentSpaceLightingSc2Uv();
				renderUnit->m_vs = myVS;

				object->GetWorldMatrix(myVS->worldMatrix);
				MatrixTranspose(myVS->inverseWorldMatrix, myVS->worldMatrix);	// This only works when there is no translation 
																				// or scaling!!!
				MatrixMultiply(myVS->worldViewProjMatrix, myVS->worldMatrix, m_activeCamera->GetViewMatrix());
				MatrixMultiply(myVS->worldViewProjMatrix, myVS->worldViewProjMatrix, m_activeCamera->GetProjMatrix());

				myVS->lightPosition = m_activeLight->position;
			}
			break;

		default:
			break;
		}

		switch (pixelShaderId)
		{
		case PS_FIXED_FUNCTION:
			{
				PsFixedFunction* myPS = new PsFixedFunction();
				renderUnit->m_ps = myPS;

				myPS->baseTexture = m_textureManager->GetTexture(material->baseTextureId);
			}
			break;

		case PS_NORMAL_MAP:
			{
				PsNormalMap* myPS = new PsNormalMap();
				renderUnit->m_ps = myPS;

				myPS->baseTexture = m_textureManager->GetTexture(material->baseTextureId);
				myPS->normalTexture = m_textureManager->GetTexture(material->bumpTextureId);
				myPS->diffuseColor = m_activeLight->diffuseColor;
				myPS->ambientColor = m_activeLight->ambientColor;
			}
			break;

		case PS_TOON_LIGHTING:
			{
				PsToonLighting* myPS = new PsToonLighting();
				renderUnit->m_ps = myPS;

				myPS->baseTexture = m_textureManager->GetTexture(material->baseTextureId);
				myPS->diffuseColor = m_activeLight->diffuseColor;
				myPS->ambientColor = m_activeLight->ambientColor;
			}
			break;

		default:
			break;
		}

		m_renderer->AddRenderUnit(renderUnit);
	}

	m_backBuffer->Clear();
	m_depthBuffer->Clear();

	m_renderer->SetRenderTarget(m_backBuffer, m_depthBuffer);
	m_renderer->Render();

	m_backBuffer->Present();
}
