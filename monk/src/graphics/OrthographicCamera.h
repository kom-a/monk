#pragma once

#include <MML/MML.h>

namespace monk
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float near, float far);
		OrthographicCamera(const mml::mat4& projectionMatrix);

		void SetPosition(const mml::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		void SetProjection(float left, float right, float bottom, float top, float near, float far);
		void SetProjection(const mml::mat4& projectionMatrix);
		
		const mml::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }
	private:
		void RecalculateViewMatrix();

	private:
		mml::vec3 m_Position = mml::vec3(0.0f);
		mml::mat4 m_ProjectionMatrix;
		mml::mat4 m_ViewMatrix = mml::mat4(1.0f);
		mml::mat4 m_ProjectionViewMatrix;
	};
}
