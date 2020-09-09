#include "ThirdPersonCamera.h"
#include "Window/Window.h"
#include "Input/Input.h"
#include "Events/Events.h"

namespace ifr
{
	ThirdPersonCamera::ThirdPersonCamera(TransformComponent* target, float zNear, float zFar, float fov) : Target(target), m_zNear(zNear), m_zFar(zFar), m_Fov(fov)
	{
		m_Yaw += 90.0f;
		m_Position.y = 14;
		m_Pitch = 40.0f;

		UpdateProjectionMatrix();
		Update();
	}

	void ThirdPersonCamera::Update()
	{
		if (!Target) return;

		ClampAngles();
		CalculatePosition();
		UpdateViewMatrix();
	}
	
	void ThirdPersonCamera::OnWindowResize(unsigned int width, unsigned int height)
	{
		UpdateProjectionMatrix();
	}
	
	void ThirdPersonCamera::UpdateProjectionMatrix()
	{
		float aspect_ratio = (float)Window::Get().GetWidth() / (float)Window::Get().GetHeight();
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

	void ThirdPersonCamera::UpdateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, Target->Position + glm::vec3(0, 4, 0), WORLD_UP_DIR);
	}

	void ThirdPersonCamera::CalculatePosition()
	{
		m_Position.x = Target->Position.x - sinf(Target->Rotation.y - AngleAroundPlayer) * DistanceFromPlayer;
		m_Position.z = Target->Position.z - cosf(Target->Rotation.y - AngleAroundPlayer) * DistanceFromPlayer;

		m_Position.y = Target->Position.y + sinf(glm::radians(m_Pitch)) * DistanceFromPlayer;
	}

	void ThirdPersonCamera::ClampAngles()
	{
		if (DistanceFromPlayer < MinDistanceFromPlayer)
			DistanceFromPlayer = MinDistanceFromPlayer;
		else if (DistanceFromPlayer > MaxDistanceFromPlayer)
			DistanceFromPlayer = MaxDistanceFromPlayer;

		if (m_Pitch < 4)
			m_Pitch = 4;
		else if (m_Pitch > 90)
			m_Pitch = 90;

		if (AngleAroundPlayer < -180.0f)
			AngleAroundPlayer += 180.0f;
		else if (AngleAroundPlayer >= 180.0f)
			AngleAroundPlayer -= 180.0f;
	}

	void ThirdPersonCamera::OnEvent(Event& e)
	{
		if (!Target) return;

		if (e.GetEventType() == EventType::MouseMoved)
		{
			MouseMovedEvent& mme = static_cast<MouseMovedEvent&>(e);

			float timestep = (float)Time::GetTimestep();

			if (IsTargetAffected)
				Target->Rotation.y -= mme.GetDX() / 2000.0f * RotationSpeed;
			else
				AngleAroundPlayer += mme.GetDX() / 2000.0f * RotationSpeed;


			m_Pitch += mme.GetDY() / 60.0f * RotationSpeed;
		}

		if (e.GetEventType() == EventType::MouseScrolled)
		{
			auto mse = static_cast<MouseScrolledEvent&>(e);

			DistanceFromPlayer -= mse.GetYOffset();
		}
	}
}
