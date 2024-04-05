#include "CameraController.h"

#include <cmath>

#include "Input/Input.h"
#include "core/Log.h"

namespace monk
{
	CameraController::CameraController(Shared<Camera> camera)
	{
		m_Camera = camera;
		m_LookDirection = camera->GetLookDirection();
		m_UpDirection = mmath::vec3(0.0f, 1.0f, 0.0f);
		m_RightDirection = mmath::Normalize(mmath::Cross(m_UpDirection, m_LookDirection));
	}

	void CameraController::Update(float deltaTime)
	{
		UpdateMoving(deltaTime);
		UpdateLooking(deltaTime);
	}

	void CameraController::UpdateMoving(float deltaTime)
	{
		if (!Input::IsMouseButtonPressed(Mouse::ButtonRight))
			return;

		const float speed = m_MoveSpeed * deltaTime;
		mmath::vec3 moveDirection(0.0f);

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

		mmath::vec3 cameraPosition = m_Camera->GetPosition();
		cameraPosition += moveDirection;
		m_Camera->SetPosition(cameraPosition);
		m_Camera->SetTarget(cameraPosition + m_LookDirection);
	}

	void CameraController::UpdateLooking(float deltaTime)
	{
		// FIXME: Center calculating wrong if mouse is moving while pressing right mouse button

		if (!Input::IsKeyPressed(Mouse::ButtonRight))
		{
			m_FirstMouseInteration = true;
			m_CenterMousePosition = mmath::vec2(-1.0f);
			return;
		}
		
		if (m_FirstMouseInteration)
		{
			m_FirstMouseInteration = false;
			return;
		}

		mmath::vec2 mousePosition = Input::GetMousePosition();

		if (m_CenterMousePosition == mmath::vec2(-1.0f))
		{
			m_CenterMousePosition = mousePosition;
			return;
		}
		
		if (mousePosition == m_CenterMousePosition)
			return;

		
		mmath::vec2 mouseOffset = m_CenterMousePosition - mousePosition;
		m_Yaw += mouseOffset.x * m_Sensitivity;
		m_Pitch += mouseOffset.y * m_Sensitivity;
		m_Pitch = mmath::Clamp(m_Pitch, -89.0f, 89.0f);

		m_LookDirection.x = std::cos(mmath::ToRadians(m_Yaw)) * std::cos(mmath::ToRadians(m_Pitch));
		m_LookDirection.y = std::sin(mmath::ToRadians(m_Pitch));
		m_LookDirection.z = std::sin(mmath::ToRadians(m_Yaw)) * std::cos(mmath::ToRadians(m_Pitch));
		m_LookDirection.normalize();

		const mmath::vec3& cameraPosition = m_Camera->GetPosition();
		m_Camera->SetTarget(cameraPosition + m_LookDirection);
		m_RightDirection = mmath::Normalize(mmath::Cross(m_UpDirection, m_LookDirection));
	}
}