#pragma once
#include <ecs/IComponent.h>
#include <math/IFRMath.h>

namespace ifr
{
	class TransformComponent : public IComponent
	{
	public:
		TransformComponent() = default;

		// Inherited via IComponent
		virtual ComponentType GetType() override;
		virtual void OnAttach() override;
		virtual void OnUpdate() override;

		glm::vec3 Position = { 0, 0, 0 };
		glm::vec3 Rotation = { 0, 0, 0 };
		glm::vec3 Scale = { 1, 1, 1 };

		glm::vec3 GetDirection();
		glm::vec3 GetVelocity();
		glm::vec3 GetRightVector(const glm::vec3& front);

	private:
		glm::vec3 m_PreviousPosition = { 0, 0, 0 };
	};
}
