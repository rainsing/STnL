/********************************************************************
	created:	2012/09/18
	created:	18:9:2012   12:10
	filename: 	e:\GitHub\STnL\Workspace\STnL\Camera.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Camera
	file ext:	cpp
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "Camera.h"

Camera::Camera(const Vector3& position, const Vector3& lookAt, float nearClipDistance, float farClipDistance, float fov, float aspect)
	:m_init_position(position), m_init_lookAt(lookAt)
{
	m_position = position;
	m_lookAt = lookAt;
	m_up = Vector3(0.0f, 1.0f, 0.0f);

	Vector3 viewDir = m_lookAt - m_position;
	Vector3 right = m_up.Cross(viewDir);
	m_up = viewDir.Cross(right);
	m_up.Normalize();

	m_distanceToLookAt = (m_lookAt - m_position).Length();

	m_nearClipDistance = nearClipDistance;
	m_farClipDistance = farClipDistance;
	m_verticalFov = fov;
	m_aspect = aspect;

	m_viewMatrixDirty = m_projMatrixDirty = true;
}

const Matrix4& Camera::GetViewMatrix( void )
{
	if (!m_viewMatrixDirty)
	{
		return m_viewMatrix;
	}

	Vector3 viewDir = m_lookAt - m_position;
	viewDir.Normalize();

	Vector3 right = m_up.Cross(viewDir);
	right.Normalize();

	Vector3 yAxis = viewDir.Cross(right);

	float tx = -m_position.Dot(right);
	float ty = -m_position.Dot(yAxis);
	float tz = -m_position.Dot(viewDir);

	m_viewMatrix.m11 = right.x;		m_viewMatrix.m12 = yAxis.x;		m_viewMatrix.m13 = viewDir.x;	m_viewMatrix.m14 = 0.0f;
	m_viewMatrix.m21 = right.y;		m_viewMatrix.m22 = yAxis.y;		m_viewMatrix.m23 = viewDir.y;	m_viewMatrix.m24 = 0.0f;
	m_viewMatrix.m31 = right.z;		m_viewMatrix.m32 = yAxis.z;		m_viewMatrix.m33 = viewDir.z;	m_viewMatrix.m34 = 0.0f;
	m_viewMatrix.m41 = tx;			m_viewMatrix.m42 = ty;			m_viewMatrix.m43 = tz;			m_viewMatrix.m44 = 1.0f;

	m_viewMatrixDirty = false;

	return m_viewMatrix;
}

const Matrix4& Camera::GetProjMatrix( void )
{
	if (!m_projMatrixDirty)
	{
		return m_projMatrix;
	}

	MakeProjectionMatrix(m_projMatrix, m_nearClipDistance, m_farClipDistance, m_verticalFov, m_aspect);
	m_projMatrixDirty = false;

	return m_projMatrix;
}

void Camera::LocalMove( float x, float y, float z )
{
	Matrix4 rotationMatrix = GetViewMatrix();
	rotationMatrix.m41 = rotationMatrix.m42 = rotationMatrix.m43 = 0.0f;

	Matrix4 inverseRotationMatrix;
	MatrixTranspose(inverseRotationMatrix, rotationMatrix);

	Vector4 worldSpaceOffset = inverseRotationMatrix.Transform(Vector4(x, y, z, 1.0f));

	m_position.x += worldSpaceOffset.x;
	m_position.y += worldSpaceOffset.y;
	m_position.z += worldSpaceOffset.z;

	m_lookAt.x += worldSpaceOffset.x;
	m_lookAt.y += worldSpaceOffset.y;
	m_lookAt.z += worldSpaceOffset.z;

	m_viewMatrixDirty = true;
}

void Camera::LocalRotate( float x, float y )
{
	Matrix4 viewSpaceRotation;
	MakeRotationMatrixX(viewSpaceRotation, x);
	
	Matrix4 viewSpaceRotationY;
	MakeRotationMatrixY(viewSpaceRotationY, y);

	MatrixMultiply(viewSpaceRotation, viewSpaceRotation, viewSpaceRotationY);

	Vector4& newViewDirection = viewSpaceRotation.Transform(Vector3(0.0f, 0.0f, 1.0f));

	Matrix4 cameraRotation = GetViewMatrix();
	cameraRotation.m41 = cameraRotation.m42 = cameraRotation.m43 = 0.0f;

	Matrix4 inverseCameraRotation;
	MatrixTranspose(inverseCameraRotation, cameraRotation);

	Vector4& newWorldSpaceViewDir = inverseCameraRotation.Transform(newViewDirection);

	Vector3 newWorldSpaceViewDir3;
	newWorldSpaceViewDir3.x = newWorldSpaceViewDir.x;
	newWorldSpaceViewDir3.y = newWorldSpaceViewDir.y;
	newWorldSpaceViewDir3.z = newWorldSpaceViewDir.z;
	newWorldSpaceViewDir3.Normalize();

	m_position = m_lookAt - newWorldSpaceViewDir3 * m_distanceToLookAt;

	Vector3 right = m_up.Cross(newWorldSpaceViewDir3);
	m_up = newWorldSpaceViewDir3.Cross(right);
	m_up.Normalize();

	m_viewMatrixDirty = true;
}

void Camera::Reset( void )
{
	m_position = m_init_position;
	m_lookAt = m_init_lookAt;
	m_up = Vector3(0.0f, 1.0f, 0.0f);

	Vector3 viewDir = m_lookAt - m_position;
	Vector3 right = m_up.Cross(viewDir);
	m_up = viewDir.Cross(right);
	m_up.Normalize();

	m_viewMatrixDirty = true;
}
