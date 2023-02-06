#pragma once

#include "Math/Math.h"

namespace monk
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float near, float far);
		OrthographicCamera(const math::mat4& projectionMatrix);
		~OrthographicCamera();

		void SetPosition(const math::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const math::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }
	private:
		void RecalculateViewMatrix();

	private:
		math::vec3 m_Position = math::vec3(0.0f);
		math::mat4 m_ProjectionMatrix;
		math::mat4 m_ViewMatrix = math::mat4(1.0f);
		math::mat4 m_ProjectionViewMatrix;
	};
}
