#include "OrthographicCamera.h"

namespace monk
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
	{
		m_ProjectionMatrix = mmath::Ortho(left, right, bottom, top, near, far);
		RecalculateViewMatrix();
	}

	OrthographicCamera::OrthographicCamera(const mmath::mat4& projectionMatrix)
	{
		m_ProjectionMatrix = projectionMatrix;
		RecalculateViewMatrix();
	}

	OrthographicCamera::~OrthographicCamera()
	{

	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		// TODO: replace this with m_ViewMatrix = mmath::Transalte(mmath::mat4(1.0f), m_Position);
		m_ViewMatrix = mmath::mat4(1.0f);
		m_ViewMatrix.Translate(-m_Position);
		m_ProjectionViewMatrix = m_ViewMatrix * m_ProjectionMatrix;
	}
}