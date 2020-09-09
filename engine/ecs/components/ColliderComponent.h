#pragma once
#include "TransformComponent.h"
#include <physics/BoundingVolumes.h>

namespace ifr
{
	class ColliderComponent : public IComponent
	{
	public:
		ColliderComponent() = default;

		// Inherited via IComponent
		virtual ComponentType GetType() override;
		virtual void OnAttach() override;
		virtual void OnUpdate() override;

		TransformComponent* GetTransformComponent() { return m_TransformComponentLink; }

		Ref<OBB> ObjectOrientedBox = nullptr;

		bool ShouldRenderBoundingVolume = false;
		float Weight = 1.0f;
		bool OnGround = false;
		bool IsAffectedByGravity = true;

		std::function<bool(ColliderComponent&)> CollisionResponseCallback = nullptr;

		glm::vec3 Velocity = glm::vec3(0, 0, 0);

	private:
		TransformComponent* m_TransformComponentLink = nullptr;
	};
}
