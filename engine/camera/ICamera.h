#pragma once
#include <math/IFRMath.h>
#include <time/Time.h>
#include <events/Event.h>

namespace ifr
{
	class ICamera
	{
	public:
		virtual void Update() = 0;
		virtual void OnWindowResize(unsigned int width, unsigned int height) = 0;
		virtual void OnEvent(Event& e) = 0;

		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		const glm::vec3& GetPosition()	 const { return m_Position; }
		const glm::vec3 GetYawPitchRoll()  const { return glm::vec3(m_Yaw, m_Pitch, m_Roll); }

		const float GetYaw()   const { return m_Yaw; }
		const float GetPitch() const { return m_Pitch; }
		const float GetRoll()  const { return m_Roll; }

		void SetPosition(const glm::vec3& position);
		void SetYawPitchRoll(const glm::vec3& orientation);

		void Translate(const glm::vec3& translation);
		void Rotate(const glm::vec3& rotation);

		void SetYaw(float yaw);
		void SetPitch(float pitch);
		void SetRoll(float roll);

		void InvertPitch();

	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		virtual void UpdateProjectionMatrix() = 0;
		virtual void UpdateViewMatrix() = 0;

	protected:
		glm::vec3 m_Position = glm::vec3(0, 0, 0);
		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;
		float m_Roll = 0.0f;
	};
}
