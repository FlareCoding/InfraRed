#include "OrthographicCamera.h"

#include "Input/Input.h"
#include "Events/KeyboardEvents.h"

#include "Window/Window.h"

namespace ifr
{
	OrthographicCamera::OrthographicCamera(float zNear, float zFar)
		: zNear(zNear), zFar(zFar)
	{
		UpdateProjectionMatrix();
		UpdateViewMatrix();
	}

	void OrthographicCamera::Update()
	{
		float frame_velocity = velocity * (float)Time::GetTimestep();

		if (Input::IsKeyPressed(IFR_KEY_S))
		{
			Translate(glm::vec3(0, -frame_velocity, 0));
		}

		if (Input::IsKeyPressed(IFR_KEY_W))
		{
			Translate(glm::vec3(0, frame_velocity, 0));
		}

		if (Input::IsKeyPressed(IFR_KEY_D))
		{
			Translate(glm::vec3(frame_velocity, 0, 0));
		}

		if (Input::IsKeyPressed(IFR_KEY_A))
		{
			Translate(glm::vec3(-frame_velocity, 0, 0));
		}
	}

	void OrthographicCamera::UpdateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::ortho(0.0f, (float)Window::Get().GetWidth(), 0.0f, (float)Window::Get().GetHeight(), zNear, zFar);
	}

	void OrthographicCamera::UpdateViewMatrix()
	{
		m_ViewMatrix = glm::mat4(1.0f);

		m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Pitch), glm::vec3(1, 0, 0));
		m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Yaw),	glm::vec3(0, 1, 0));
		m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Roll),	glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::translate(m_ViewMatrix, -m_Position);
	}

	void OrthographicCamera::OnWindowResize(unsigned int width, unsigned int height)
	{
		UpdateProjectionMatrix();
	}
}
