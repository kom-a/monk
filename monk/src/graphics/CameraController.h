#pragma once

#include "graphics/Camera.h"
#include <MML/MML.h>
#include "core/Memory.h"

namespace monk
{
	class CameraController
	{
	public:
		CameraController(Ref<Camera> camera);

	public:
		void Update(float deltaTime);

	private:
		void UpdateMoving(float deltaTime);
		void UpdateLooking(float deltaTime);

	private:
		Ref<Camera> m_Camera = nullptr;
		mml::vec3 m_LookDirection;
		mml::vec3 m_RightDirection;
		mml::vec3 m_UpDirection;
		float m_Yaw = 90, m_Pitch = 0;
		float m_Sensitivity = 0.05f;
		float m_MoveSpeed = 7.0f;

		bool m_FirstMouseInteration = true;
		mml::vec2 m_CenterMousePosition = mml::vec2(-1.0f);
	};
}