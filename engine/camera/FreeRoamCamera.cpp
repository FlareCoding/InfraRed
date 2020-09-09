#include "FreeRoamCamera.h"
#include "Window/Window.h"
#include "Input/Input.h"

namespace ifr
{
	FreeRoamCamera::FreeRoamCamera(float zNear, float zFar, float fov)
		: m_zNear(zNear), m_zFar(zFar), m_Fov(fov)
	{
		m_WorldUp = glm::vec3(0, 1, 0);
		m_Front = glm::vec3(0, 0, 1);

		m_Yaw += 180.0f;

		UpdateProjectionMatrix();
		UpdateViewMatrix();
	}

	void FreeRoamCamera::Update()
	{
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
		else if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;

		float frame_velocity = Speed * (float)Time::GetTimestep() / 1000.0f;

		if (!RestrictMovement)
		{
			if (Input::IsKeyPressed(IFR_KEY_W))
				m_Position += m_Front * frame_velocity;

			if (Input::IsKeyPressed(IFR_KEY_S))
				m_Position -= m_Front * frame_velocity;

			if (Input::IsKeyPressed(IFR_KEY_A))
				m_Position -= m_Right * frame_velocity;

			if (Input::IsKeyPressed(IFR_KEY_D))
				m_Position += m_Right * frame_velocity;
		}

		UpdateViewMatrix();
	}
	
	void FreeRoamCamera::OnWindowResize(unsigned int width, unsigned int height)
	{
		UpdateProjectionMatrix();
	}
	
	void FreeRoamCamera::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::MouseMoved)
		{
			float frame_rotation_velocity = RotationSpeed * (float)Time::GetTimestep();
			MouseMovedEvent& mme = static_cast<MouseMovedEvent&>(e);

			float timestep = (float)Time::GetTimestep();

			m_Yaw += mme.GetDX() / 1000.0f * frame_rotation_velocity;
			m_Pitch -= mme.GetDY() / 1000.0f * frame_rotation_velocity;
		}
	}

	void FreeRoamCamera::UpdateProjectionMatrixDimensions(unsigned int view_width, unsigned int view_height)
	{
		m_ViewWidth = view_width;
		m_ViewHeight = view_height;

		UpdateProjectionMatrix();
	}
	
	void FreeRoamCamera::UpdateProjectionMatrix()
	{
		if (m_ViewWidth == 0) m_ViewWidth = Window::Get().GetWidth();
		if (m_ViewHeight == 0) m_ViewHeight = Window::Get().GetHeight();

		float aspect_ratio = (float)m_ViewWidth / (float)m_ViewHeight;
		float y_scale = (float)((1.f / tanf(glm::radians(m_Fov / 2.f))) * aspect_ratio);
		float x_scale = y_scale / aspect_ratio;
		float frustum_length = m_zFar - m_zNear;

		m_ProjectionMatrix = glm::mat4(1.0f);
		m_ProjectionMatrix[0][0] = x_scale;
		m_ProjectionMatrix[1][1] = y_scale;
		m_ProjectionMatrix[2][2] = -((m_zFar + m_zNear) / frustum_length);
		m_ProjectionMatrix[2][3] = -1;
		m_ProjectionMatrix[3][2] = -((2 * m_zNear * m_zFar) / frustum_length);
		m_ProjectionMatrix[3][3] = 0;
	}
	
	void FreeRoamCamera::UpdateViewMatrix()
	{
		glm::vec3 front;
		front.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
		front.y = sinf(glm::radians(m_Pitch));
		front.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);

		m_Right = glm::normalize(glm::cross(m_Front, WORLD_UP_DIR));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}
}
