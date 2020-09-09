#pragma once
#include "ICamera.h"

namespace ifr
{
	class OrthographicCamera : public ICamera
	{
	public:
		OrthographicCamera(float zNear = 1000.0f, float zFar = 0.0f);

		// Inherited via Camera
		virtual void Update() override;
		virtual void OnWindowResize(unsigned int width, unsigned int height) override;

	private:
		float velocity = 0.6f;
		float zNear, zFar;

	private:
		// Inherited via Camera
		virtual void UpdateProjectionMatrix() override;
		virtual void UpdateViewMatrix() override;
		virtual void OnEvent(Event& e) override {}
	};
}
