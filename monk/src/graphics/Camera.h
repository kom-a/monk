#pragma once

#include "Math/Math.h"

namespace monk
{
	class Camera
	{
	public:
		Camera(float fov, float aspectRatio, float nearPlane, float farPlane);

		void SetPosition(const math::vec3& position);
		void SetTarget(const math::vec3& target);
		void SetAspectRatio(float aspectRatio);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		const math::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; };
		const math::vec3& GetPosition() const { return m_Position; }
		const math::vec3& GetLookDirection() const { return math::Normalize(m_Target - m_Position); }

	private:
		void RecalculateProjectionMatrix();
		void RecalculateViewMatrix();

	private:
		math::vec3 m_Position;
		math::vec3 m_Target;
		math::mat4 m_ProjectionMatrix;
		math::mat4 m_ViewMatrix;
		math::mat4 m_ProjectionViewMatrix;
		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	};
}