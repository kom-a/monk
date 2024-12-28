#pragma once

#include <MML/MML.h>

namespace monk
{
	class Camera
	{
	public:
		Camera(float fov, float aspectRatio, float nearPlane, float farPlane);

		void SetPosition(const mml::vec3& position);
		void SetTarget(const mml::vec3& target);
		void SetAspectRatio(float aspectRatio);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		const mml::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; };
		const mml::vec3& GetPosition() const { return m_Position; }
		const mml::vec3& GetLookDirection() const { return mml::Normalize(m_Target - m_Position); }

	private:
		void RecalculateProjectionMatrix();
		void RecalculateViewMatrix();

	private:
		mml::vec3 m_Position;
		mml::vec3 m_Target;
		mml::mat4 m_ProjectionMatrix;
		mml::mat4 m_ViewMatrix;
		mml::mat4 m_ProjectionViewMatrix;
		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	};
}