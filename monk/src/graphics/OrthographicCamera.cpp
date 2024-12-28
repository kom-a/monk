#include "OrthographicCamera.h"

namespace monk
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
	{
		m_ProjectionMatrix = mml::Ortho(left, right, bottom, top, near, far);
		RecalculateViewMatrix();
	}

	OrthographicCamera::OrthographicCamera(const mml::mat4& projectionMatrix)
	{
		m_ProjectionMatrix = projectionMatrix;
		RecalculateViewMatrix();
	}

	OrthographicCamera::~OrthographicCamera()
	{

	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		// TODO: replace this with m_ViewMatrix = mml::Transalte(mml::mat4(1.0f), m_Position);
		m_ViewMatrix = mml::mat4(1.0f);
		m_ViewMatrix.Translate(-m_Position);
		m_ProjectionViewMatrix = m_ViewMatrix * m_ProjectionMatrix;
	}
}