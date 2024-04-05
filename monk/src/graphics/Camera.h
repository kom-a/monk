#pragma once

#include <MMath/MMath.h>

namespace monk
{
	class Camera
	{
	public:
		Camera(float fov, float aspectRatio, float nearPlane, float farPlane);

		void SetPosition(const mmath::vec3& position);
		void SetTarget(const mmath::vec3& target);
		void SetAspectRatio(float aspectRatio);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		const mmath::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; };
		const mmath::vec3& GetPosition() const { return m_Position; }
		const mmath::vec3& GetLookDirection() const { return mmath::Normalize(m_Target - m_Position); }

	private:
		void RecalculateProjectionMatrix();
		void RecalculateViewMatrix();

	private:
		mmath::vec3 m_Position;
		mmath::vec3 m_Target;
		mmath::mat4 m_ProjectionMatrix;
		mmath::mat4 m_ViewMatrix;
		mmath::mat4 m_ProjectionViewMatrix;
		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	};
}