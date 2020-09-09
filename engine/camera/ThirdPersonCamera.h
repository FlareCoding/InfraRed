#pragma once
#include "ICamera.h"
#include <ecs/components/TransformComponent.h>

namespace ifr
{
	class ThirdPersonCamera : public ICamera
	{
	public:
		ThirdPersonCamera(TransformComponent* target, float zNear = 0.1f, float zFar = 2000.0f, float fov = 90.0f);

		// Inherited via Camera
		virtual void Update() override;
		virtual void OnWindowResize(unsigned int width, unsigned int height) override;
		virtual void OnEvent(Event& e) override;

		TransformComponent* Target = nullptr;
		bool IsTargetAffected = true;

		float RotationSpeed = 1.4f;
		float DistanceFromPlayer = 10;
		float MinDistanceFromPlayer = 4;
		float MaxDistanceFromPlayer = 14;

		float AngleAroundPlayer = 0;

	private:
		float m_zNear = 0, m_zFar = 0, m_Fov = 0;

		// Inherited via Camera
		virtual void UpdateProjectionMatrix() override;
		virtual void UpdateViewMatrix() override;

	private:
		void CalculatePosition();
		void ClampAngles();
	};
}