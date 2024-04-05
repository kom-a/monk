#pragma once

#include <MMath/MMath.h>

namespace monk
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float near, float far);
		OrthographicCamera(const mmath::mat4& projectionMatrix);
		~OrthographicCamera();

		void SetPosition(const mmath::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const mmath::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }
	private:
		void RecalculateViewMatrix();

	private:
		mmath::vec3 m_Position = mmath::vec3(0.0f);
		mmath::mat4 m_ProjectionMatrix;
		mmath::mat4 m_ViewMatrix = mmath::mat4(1.0f);
		mmath::mat4 m_ProjectionViewMatrix;
	};
}
