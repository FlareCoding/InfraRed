#include "Scene.h"
#include <window/Window.h>
#include <rendering/Renderer3D.h>
#include <ecs/systems/RenderComponentSystem.h>
#include <ecs/systems/PhysicsSystem.h>
#include <ecs/systems/ScriptingSystem.h>
#include <ecs/ComponentManager.h>
#include <camera/OrthographicCamera.h>
#include <algorithm>
#include <varargs.h>

namespace ifr
{
	uint32_t Scene::RECURSION_COUNT = 400;
	uint32_t Scene::RAY_RANGE = 800;

	static constexpr auto REMOVE_ENTITY_FROM_SCENE_FL = 0x01;
	static constexpr auto DELETE_ENTITY_FL = 0x02;

	Ref<Scene> Scene::Create(const std::string& name, const Ref<ICamera>& camera, const Ref<Terrain>& terrain, Light light)
	{
		return Ref<Scene>(new Scene(name, camera, terrain, light));
	}

	void Scene::Erase()
	{
		for (auto& pair : m_EntityMap)
			RemoveAndDeleteEntity(pair.first);

		m_Terrain = nullptr;
		m_Skybox = nullptr;
		m_Camera = nullptr;
	}

	Scene::Scene(const std::string& name, const Ref<ICamera>& camera, const Ref<Terrain>& terrain, Light light) : m_Name(name), m_Camera(camera), m_Terrain(terrain), m_Light(light)
	{
		m_ViewportOffset = { 0, 0 };
		m_ViewportSize = { Window::Get().GetWidth(), Window::Get().GetHeight() };
	}

	void Scene::Update()
	{
		if (!m_Active) return;

		CheckEntityRemovalPool();

		if (!m_Camera)
		{
			IFR_LOG(m_Name + "> [-] Camera Missing [-]");
			return;
		}

		PhysicsSystem::Update();
		ScriptingSystem::Update();

		if (m_WaterChunk)
			RenderContentsWithWater();
		else
			RenderContents();
	}

	void Scene::RenderContents(const glm::vec4& ClipPlane)
	{
		Renderer3D::BeginScene(*m_Camera, m_Light);
		RenderScene(ClipPlane);
		Renderer3D::EndScene();
	}

	void Scene::RenderContentsWithWater()
	{
		// Adjusting the camera position to be "under the water"
		float camera_displacement = 2.0f * (m_Camera->GetPosition().y - m_WaterChunk->Position.y);
		m_Camera->Translate({0, -camera_displacement, 0 });
		m_Camera->InvertPitch();

		// Begin reflection scene
		Renderer3D::BeginScene(*m_Camera, m_Light);

		// Rendering to the reflection buffer
		m_WaterChunk->BindReflectionBuffer();
		RenderScene(glm::vec4(0, 1, 0, -m_WaterChunk->Position.y));

		// Restoring camera's original position
		m_Camera->Translate({ 0, camera_displacement, 0 });
		m_Camera->InvertPitch();

		// End reflection scene
		Renderer3D::EndScene();

		// Begin refraction scene and normal scene rendering because camera position is the same for both
		Renderer3D::BeginScene(*m_Camera, m_Light);

		// Rendering to the refraction buffer
		m_WaterChunk->BindRefractionBuffer();
		RenderScene(glm::vec4(0, -1, 0, m_WaterChunk->Position.y));

		// Rendering the full scene to the main buffer
		m_WaterChunk->RestoreMainBackBuffer(m_ViewportOffset.x, m_ViewportOffset.y, m_ViewportSize.x, m_ViewportSize.y);
		RenderScene();
		m_WaterChunk->Render(*m_Camera, m_Light);
		
		Renderer3D::EndScene();
	}

	void Scene::RenderScene(const glm::vec4& ClipPlane)
	{
		if (m_Skybox && m_Skybox->texture) m_Skybox->Render(*m_Camera);
		if (m_Terrain) m_Terrain->Render(*m_Camera, m_Light);

		RenderComponentSystem::Update(ClipPlane);
	}

	void Scene::SetTerrain(const Ref<Terrain>& terrain)
	{
		m_Terrain = terrain;

		if (m_Active)
			PhysicsSystem::SetWorldTerrain(terrain);
	}

	void Scene::AddEntity(Entity e, std::initializer_list<ComponentType> active_components)
	{
		std::vector<ComponentType> component_types;

		if (m_EntityMap.find(e) == m_EntityMap.end())
		{
			for (auto& comp : active_components)
				component_types.push_back(comp);

			m_EntityMap[e] = component_types;
		}
		else
		{
			component_types = m_EntityMap[e];
		}

		// acitvating all components if scene is active
		if (m_Active)
			for (auto& component_type : component_types)
				GetComponent(e, component_type)->Enabled = true;
	}

	void Scene::AddEntity(Entity e, const std::vector<ComponentType>& active_components)
	{
		std::vector<ComponentType> component_types;

		if (m_EntityMap.find(e) == m_EntityMap.end())
		{
			for (auto& comp : active_components)
				component_types.push_back(comp);

			m_EntityMap[e] = component_types;
		}
		else
		{
			component_types = m_EntityMap[e];
		}

		// acitvating all components if scene is active
		if (m_Active)
			for (auto& component_type : component_types)
				GetComponent(e, component_type)->Enabled = true;
	}

	void Scene::RemoveEntity(Entity e)
	{
		short removal_fl = 0;
		removal_fl |= REMOVE_ENTITY_FROM_SCENE_FL;

		m_EntityRemovalPool.push_back({ e, removal_fl });
	}

	void Scene::RemoveAndDeleteEntity(Entity e)
	{
		short removal_fl = 0;
		removal_fl |= REMOVE_ENTITY_FROM_SCENE_FL;
		removal_fl |= DELETE_ENTITY_FL;

		m_EntityRemovalPool.push_back({ e, removal_fl });
	}

	void Scene::CheckEntityRemovalPool()
	{
		if (m_EntityRemovalPool.size())
		{
			for (auto& [entity, flags] : m_EntityRemovalPool)
			{
				if (flags & REMOVE_ENTITY_FROM_SCENE_FL)
					this->RemoveEntityFromScene(entity);
				
				if (flags & DELETE_ENTITY_FL)
					DeleteEntity(entity);
			}

			m_EntityRemovalPool.clear();
		}
	}

	void Scene::RemoveEntityFromScene(Entity e)
	{
		if (m_EntityMap.find(e) != m_EntityMap.end())
		{
			for (auto& component_type : m_EntityMap[e])
				GetComponent(e, component_type)->Enabled = false;

			m_EntityMap.erase(e);
		}
	}
	
	bool Scene::GetTerrainSurfacePointFromRay(const glm::vec3& ray, glm::vec3& out_point)
	{
		if (IsIntersectionInRange(0, (float)RAY_RANGE, ray))
		{
			out_point = BinarySearchRay(0, 0, (float)RAY_RANGE, ray);
			return true;
		}
		else
			return false;
	}

	void Scene::SetActive(bool active)
	{
		m_Active = active;

		if (m_Active)
		{
			// updating the physics system to use current terrain
			PhysicsSystem::SetWorldTerrain(m_Terrain);

			// enable all components
			for (auto& [entityID, components] : m_EntityMap)
			{
				for (auto& component_type : components)
					GetComponent(entityID, component_type)->Enabled = true;
			}
		}
		else
		{
			PhysicsSystem::SetWorldTerrain(nullptr);

			// disable all components
			for (auto& [entityID, components] : m_EntityMap)
			{
				for (auto& component_type : components)
					GetComponent(entityID, component_type)->Enabled = false;
			}
		}
	}

	Entity Scene::FindClosestEntity(const glm::vec3& ray, const glm::vec3& ray_position)
	{
		Entity entity_found = INVALID_ENTITY;

		ColliderComponentManager.ForEachComponent([&entity_found , &ray, &ray_position](Entity entity, ColliderComponent& collider) {
			if (!collider.Enabled) return;

			if (entity_found == INVALID_ENTITY)
				if (Physics::IsIntersecting(*collider.ObjectOrientedBox, ray, ray_position))
					entity_found = entity;
		});

		return entity_found;
	}

	void Scene::ForEntityInScene(const std::function<void(Entity)>& callback)
	{
		for (auto& pair : m_EntityMap)
			callback(pair.first);
	}

	bool Scene::IsUnderGround(const glm::vec3& test_point)
	{
		float height = 0;
		if (m_Terrain) 
			height = m_Terrain->GetSurfaceHeight(test_point.x, test_point.z);

		return test_point.y < height;
	}

	bool Scene::IsIntersectionInRange(float start, float finish, const glm::vec3& ray)
	{
		auto start_point = GetPointOnRay(ray, start);
		auto end_point = GetPointOnRay(ray, finish);
		
		return (!IsUnderGround(start_point) && IsUnderGround(end_point));
	}

	glm::vec3 Scene::GetPointOnRay(const glm::vec3& ray, float distance)
	{
		auto camera_pos = m_Camera->GetPosition();
		auto start = glm::vec3(camera_pos.x, camera_pos.y, camera_pos.z);
		auto scaled_ray = glm::vec3(ray.x * distance, ray.y * distance, ray.z * distance);
		return start + scaled_ray;
	}

	glm::vec3 Scene::BinarySearchRay(int count, float start, float finish, const glm::vec3& ray)
	{
		float half = start + ((finish - start) / 2.0f);

		if (count >= (float)RECURSION_COUNT)
		{
			auto end_point = GetPointOnRay(ray, half);
			
			if (m_Terrain)
				return end_point;
			else
				return glm::vec3(0, 0, 0);
		}

		if (IsIntersectionInRange(start, half, ray))
			return BinarySearchRay(count + 1, start, half, ray);

		else
			return BinarySearchRay(count + 1, half, finish, ray);
	}
}
