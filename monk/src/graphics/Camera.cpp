#include "Camera.h"

namespace monk
{

	Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
		:
		m_Position(math::vec3(0.0f)),
		m_Target(math::vec3(0.0f, 0.0f, 0.0f)),
		m_FOV(fov), 
		m_AspectRatio(aspectRatio), 
		m_NearPlane(nearPlane), 
		m_FarPlane(farPlane)
	{
		RecalculateProjectionMatrix();
		RecalculateViewMatrix();
	}

	void Camera::SetPosition(const math::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

	void Camera::SetTarget(const math::vec3& target)
	{
		m_Target = target;
		RecalculateViewMatrix();
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		RecalculateProjectionMatrix();
	}

	void Camera::SetNearPlane(float nearPlane)
	{
		m_NearPlane = nearPlane;
		RecalculateProjectionMatrix();
	}

	void Camera::SetFarPlane(float farPlane)
	{
		m_FarPlane = farPlane;
		RecalculateProjectionMatrix();
	}

	void Camera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = math::Perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateViewMatrix()
	{
		m_ViewMatrix = math::LookAt(m_Position, m_Target);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}