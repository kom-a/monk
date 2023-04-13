#pragma once

#include "graphics/Camera.h"
#include "math/Math.h"
#include "core/Memory.h"

namespace monk
{
	class CameraController
	{
	public:
		CameraController(Shared<Camera> camera);

	public:
		void Update(float deltaTime);

	private:
		void UpdateMoving(float deltaTime);
		void UpdateLooking(float deltaTime);

	private:
		Shared<Camera> m_Camera = nullptr;
		math::vec3 m_LookDirection;
		math::vec3 m_RightDirection;
		math::vec3 m_UpDirection;
		float m_Yaw = 90, m_Pitch = 0;
		float m_Sensitivity = 0.05f;
		float m_MoveSpeed = 7.0f;

		bool m_FirstMouseInteration = true;
		math::vec2 m_CenterMousePosition = math::vec2(-1.0f);
	};
}