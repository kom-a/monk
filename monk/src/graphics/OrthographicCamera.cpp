#include "OrthographicCamera.h"

namespace monk
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
	{
		SetProjection(mml::Ortho(left, right, bottom, top, near, far));
	}

	OrthographicCamera::OrthographicCamera(const mml::mat4& projectionMatrix)
	{
		SetProjection(projectionMatrix);
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float near, float far)
	{
		m_ProjectionMatrix = mml::Ortho(left, right, bottom, top, near, far);
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(const mml::mat4& projectionMatrix)
	{
		m_ProjectionMatrix = projectionMatrix;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		// TODO: replace this with m_ViewMatrix = mml::Transalte(mml::mat4(1.0f), m_Position);
		m_ViewMatrix = mml::mat4(1.0f);
		m_ViewMatrix.Translate(-m_Position);
		m_ProjectionViewMatrix = m_ViewMatrix * m_ProjectionMatrix;
	}
}