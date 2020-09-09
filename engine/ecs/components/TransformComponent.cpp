#include "TransformComponent.h"

namespace ifr
{
	ComponentType TransformComponent::GetType()
	{
		return ComponentType::Transform;
	}

	void TransformComponent::OnAttach()
	{
		this->Enabled = true;
	}

	void TransformComponent::OnUpdate()
	{
		m_PreviousPosition = Position;
	}
	
	glm::vec3 TransformComponent::GetDirection()
	{
		return glm::vec3(
			sin(Rotation.y),
			0,
			cos(Rotation.y)
		);
	}

	glm::vec3 TransformComponent::GetVelocity()
	{
		return (Position - m_PreviousPosition);
	}
	
	glm::vec3 TransformComponent::GetRightVector(const glm::vec3& front)
	{
		return glm::normalize(glm::cross(front, WORLD_UP_DIR));
	}
}
