#include "OrthographicCamera.h"

namespace monk
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
	{
		m_ProjectionMatrix = math::Ortho(left, right, bottom, top, near, far);
		RecalculateViewMatrix();
	}

	OrthographicCamera::OrthographicCamera(const math::mat4& projectionMatrix)
	{
		m_ProjectionMatrix = projectionMatrix;
		RecalculateViewMatrix();
	}

	OrthographicCamera::~OrthographicCamera()
	{

	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		// TODO: replace this with m_ViewMatrix = math::Transalte(math::mat4(1.0f), m_Position);
		m_ViewMatrix = math::mat4(1.0f);
		m_ViewMatrix.Translate(-m_Position);
		m_ProjectionViewMatrix = m_ViewMatrix * m_ProjectionMatrix;
	}
}