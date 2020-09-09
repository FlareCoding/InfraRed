#pragma once
#include "../Components.h"
#include "Physics/CollisionDetection.h"
#include "Scene/Terrain.h"

namespace ifr
{
	class PhysicsSystem
	{
	public:
		static void SetWorldTerrain(Ref<Terrain> terrain);
		static void Update();

	private:
		static Ref<Terrain> s_WorldTerrain;
		static float s_Gravity;

		static void AdjustTransformForCollider(ColliderComponent& collider, TransformComponent* transform);

		static void ResolvePossibleCollisions(ColliderComponent& component);
		static void ResolveCollision(ColliderComponent& first, ColliderComponent& second, glm::vec3& mtv);
	};
}
