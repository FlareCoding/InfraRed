#include "PhysicsSystem.h"
#include "Rendering/Renderer.h"
#include "Rendering/Renderer3D.h"
#include "../ComponentManager.h"

namespace ifr
{
	Ref<Terrain> PhysicsSystem::s_WorldTerrain = nullptr;
	float PhysicsSystem::s_Gravity = -2.0f;

	static std::map<Entity, bool> s_EntityIsOnGroundMap;

	void PhysicsSystem::SetWorldTerrain(Ref<Terrain> terrain)
	{
		s_WorldTerrain = terrain;
	}

	void PhysicsSystem::Update()
	{
		ColliderComponentManager.ForEachComponent([](Entity entity, ColliderComponent& component) {
			if (!component.Enabled) return;

			component.OnUpdate();

			s_EntityIsOnGroundMap[entity] = false;

			component.ObjectOrientedBox->DebugColor = glm::vec3(0, 1, 0);
			component.ObjectOrientedBox->GetAABB()->DebugColor = glm::vec3(0, 1, 0);

			if (component.ObjectOrientedBox)
				ResolvePossibleCollisions(component);

			auto* transform = component.GetTransformComponent();
			if (transform)
			{
				AdjustTransformForCollider(component, transform);

				transform->OnUpdate();
			}
		});
	}

	void PhysicsSystem::AdjustTransformForCollider(ColliderComponent& collider, TransformComponent* transform)
	{
		// Adjusting velocity due to gravity
		if (collider.IsAffectedByGravity)
		{
			auto ts = (float)Time::GetTimestep();
			collider.Velocity.y += s_Gravity / 3000.0f * ts;
		}

		// Determining if the object is in the air
		if (s_EntityIsOnGroundMap[collider.Owner])
			collider.OnGround = true;
		else
			collider.OnGround = false;

		if (collider.Velocity.y > 0)
			collider.OnGround = false;

		if (collider.OnGround && collider.Velocity.y < 0)
			collider.Velocity.y = 0;

		// Moving the object
		transform->Position += collider.Velocity;

		// Determining ground position
		float surface_height = 0;

		if (s_WorldTerrain)
			surface_height = s_WorldTerrain->GetSurfaceHeight(transform->Position.x, transform->Position.z);

		// Adjusting transform position in case the object fell through the ground
		float bounding_box_bottom = collider.ObjectOrientedBox->GetAABB()->BottomLeftFar.y;

		if (bounding_box_bottom < surface_height)
		{
			// difference between model's center and bounding box's bottom
			float box_model_position_difference = transform->Position.y - bounding_box_bottom;

			transform->Position.y = surface_height + box_model_position_difference;
			collider.Velocity.y = 0;
			collider.OnGround = true;
		}
	}

	void PhysicsSystem::ResolvePossibleCollisions(ColliderComponent& component)
	{
		ColliderComponentManager.ForEachComponent([&component](Entity entity, ColliderComponent& potential_collision_candidate) {
			if (!potential_collision_candidate.Enabled) return;

			if (potential_collision_candidate.ObjectOrientedBox && &potential_collision_candidate != &component)
			{
				bool intersecting = Physics::IsIntersecting(*component.ObjectOrientedBox->GetAABB(), *potential_collision_candidate.ObjectOrientedBox->GetAABB());

				if (intersecting)
				{
					component.ObjectOrientedBox->GetAABB()->DebugColor = glm::vec3(1, 0, 0);
					potential_collision_candidate.ObjectOrientedBox->GetAABB()->DebugColor = glm::vec3(1, 0, 0);

					glm::vec3 mtv;

					if (Physics::IsIntersectingSAT(*component.ObjectOrientedBox, *potential_collision_candidate.ObjectOrientedBox, mtv))
					{
						component.ObjectOrientedBox->DebugColor = glm::vec3(1, 0, 0);
						potential_collision_candidate.ObjectOrientedBox->DebugColor = glm::vec3(1, 0, 0);

						ResolveCollision(component, potential_collision_candidate, mtv);
					}
				}
			}
		});
	}

	void PhysicsSystem::ResolveCollision(ColliderComponent& first, ColliderComponent& second, glm::vec3& mtv)
	{
		if (first.CollisionResponseCallback)
			if (first.CollisionResponseCallback(second))
				return;

		auto pos_delta = second.ObjectOrientedBox->GetAABB()->GetCenter() - first.ObjectOrientedBox->GetAABB()->GetCenter();

		auto* first_transform = first.GetTransformComponent();
		auto* second_transform = second.GetTransformComponent();

		if (!first_transform || !second_transform) return;

		if (first.Weight <= second.Weight)
		{
			auto to_collision_object_direction = second_transform->Position - first_transform->Position;
			float dot = glm::dot(mtv, to_collision_object_direction);

			if (dot > 0) mtv *= -1;

			first_transform->Position += mtv;

			if (glm::dot(glm::normalize(mtv), WORLD_UP_DIR) > 0.8f)
			{
				s_EntityIsOnGroundMap[first.Owner] = true;
			}
		}
	}
}
