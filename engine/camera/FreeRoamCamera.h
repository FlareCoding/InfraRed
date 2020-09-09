#pragma once
#include "ICamera.h"

namespace ifr
{
	class FreeRoamCamera : public ICamera
	{
	public:
		FreeRoamCamera(float zNear = 0.1f, float zFar = 10000.0f, float fov = 90.0f);

		// Inherited via Camera
		virtual void Update() override;
		virtual void OnWindowResize(unsigned int width, unsigned int height) override;
		virtual void OnEvent(Event& e) override;
		virtual void UpdateProjectionMatrixDimensions(unsigned int view_width, unsigned int view_height);

		float Speed = 100.0f;
		float RotationSpeed = 160.0f;
		bool RestrictMovement = false;

	private:
		float m_zNear, m_zFar, m_Fov;

		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;

		uint32_t m_ViewWidth = 0, m_ViewHeight = 0;

		// Inherited via Camera
		virtual void UpdateProjectionMatrix() override;
		virtual void UpdateViewMatrix() override;
	};
}
