/********************************************************************
	created:	2012/09/18
	created:	18:9:2012   12:09
	filename: 	e:\GitHub\STnL\Workspace\STnL\Camera.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Camera
	file ext:	h
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#ifndef Camera_h__
#define Camera_h__

#include "Vector.h"
#include "Matrix.h"

class Camera
{
public:
	Camera(const Vector3& position, const Vector3& lookAt, float nearClipDistance, float farClipDistance, float fov, float aspect);

	const Matrix4& GetViewMatrix(void);
	const Matrix4& GetProjMatrix(void);

	void LocalMove(float x, float y, float z);
	void LocalRotate(float x, float y);
	void Reset(void);

private:
	Vector3 m_position;
	Vector3 m_lookAt;
	Vector3 m_up;
	float m_distanceToLookAt;

	Matrix4 m_viewMatrix;
	bool m_viewMatrixDirty;

	float m_nearClipDistance;
	float m_farClipDistance;
	float m_verticalFov;
	float m_aspect;

	Matrix4 m_projMatrix;
	bool m_projMatrixDirty;

	const Vector3 m_init_position;
	const Vector3 m_init_lookAt;
};

#endif // Camera_h__
