#include "CameraController.h"

#include <cmath>

#include "Input/Input.h"
#include "core/Log.h"

namespace monk
{
	CameraController::CameraController(Camera* camera)
	{
		m_Camera = camera;
		m_LookDirection = camera->GetLookDirection();
		m_UpDirection = math::vec3(0.0f, 1.0f, 0.0f);
		m_RightDirection = math::Normalize(math::Cross(m_UpDirection, m_LookDirection));
	}

	void CameraController::Update(float deltaTime)
	{
		UpdateMoving(deltaTime);
		UpdateLooking(deltaTime);
	}

	void CameraController::UpdateMoving(float deltaTime)
	{
		const float speed = m_MoveSpeed * deltaTime;
		math::vec3 moveDirection(0.0f);

		if (Input::IsKeyPressed(Key::W))
			moveDirection += m_LookDirection;
		if (Input::IsKeyPressed(Key::S))
			moveDirection += -m_LookDirection;
		if (Input::IsKeyPressed(Key::D))
			moveDirection += m_RightDirection;
		if (Input::IsKeyPressed(Key::A))
			moveDirection += -m_RightDirection;
		if(Input::IsKeyPressed(Key::Space))
			moveDirection += m_UpDirection;
		if (Input::IsKeyPressed(Key::LeftControl))
			moveDirection += -m_UpDirection;

		if (moveDirection.length2() <= 0.0f)
			return;

		moveDirection.normalize();
		moveDirection *= speed;

		math::vec3 cameraPosition = m_Camera->GetPosition();
		cameraPosition += moveDirection;
		m_Camera->SetPosition(cameraPosition);
		m_Camera->SetTarget(cameraPosition + m_LookDirection);
	}

	void CameraController::UpdateLooking(float deltaTime)
	{
		if (!Input::IsKeyPressed(Mouse::ButtonRight))
		{
			m_FirstMouseInteration = true;
			m_CenterMousePosition = math::vec2(-1.0f);
			return;
		}
		
		if (m_FirstMouseInteration)
		{
			m_FirstMouseInteration = false;
			return;
		}

		math::vec2 mousePosition = Input::GetMousePosition();

		if (m_CenterMousePosition == math::vec2(-1.0f))
		{
			m_CenterMousePosition = mousePosition;
			return;
		}
		
		if (mousePosition == m_CenterMousePosition)
			return;

		
		math::vec2 mouseOffset = m_CenterMousePosition - mousePosition;
		m_Yaw += mouseOffset.x * m_Sensitivity;
		m_Pitch += mouseOffset.y * m_Sensitivity;
		m_Pitch = math::Clamp(m_Pitch, -89.0f, 89.0f);

		m_LookDirection.x = std::cos(math::ToRadians(m_Yaw)) * std::cos(math::ToRadians(m_Pitch));
		m_LookDirection.y = std::sin(math::ToRadians(m_Pitch));
		m_LookDirection.z = std::sin(math::ToRadians(m_Yaw)) * std::cos(math::ToRadians(m_Pitch));
		m_LookDirection.normalize();

		const math::vec3& cameraPosition = m_Camera->GetPosition();
		m_Camera->SetTarget(cameraPosition + m_LookDirection);
		m_RightDirection = math::Normalize(math::Cross(m_UpDirection, m_LookDirection));
	}
}