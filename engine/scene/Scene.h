#pragma once
#include <camera/ICamera.h>
#include "Terrain.h"
#include "Skybox.h"
#include "Water.h"
#include "Grass.h"
#include <ecs/Entity.h>

namespace ifr
{
	class Scene
	{
		friend class IFRSceneLoader;

	public:
		static Ref<Scene> Create(const std::string& name, const Ref<ICamera>& camera = nullptr, const Ref<Terrain>& terrain = nullptr, Light light = Light());

		void Erase();

		void Update();
		void RenderContents(const glm::vec4& ClipPlane = glm::vec4(0, -1, 0, 100000));
		void RenderContentsWithWater();

		void SetViewportOffset(glm::ivec2 offset) { m_ViewportOffset = offset; }
		void SetViewportSize(glm::ivec2 size) { m_ViewportSize = size; }

		std::string GetName() const { return m_Name; }
		Ref<ICamera> GetCamera() const { return m_Camera; }
		Ref<Terrain> GetTerrain() const { return m_Terrain; }
		Ref<Skybox> GetSkybox() const { return m_Skybox; }
		Light* GetLight() { return &m_Light; }

		void SetName(const std::string& name) { m_Name = name; }
		void SetTerrain(const Ref<Terrain>& terrain);
		void SetCamera(const Ref<ICamera>& camera) { m_Camera = camera; }
		void SetLight(Light light) { m_Light = light; }
		void SetSkybox(const Ref<Skybox>& skybox) { m_Skybox = skybox; }

		void AddEntity(Entity e, std::initializer_list<ComponentType> active_components);
		void AddEntity(Entity e, const std::vector<ComponentType>& active_components);
		void RemoveEntity(Entity e);
		void RemoveAndDeleteEntity(Entity e);

		void AddWaterChunk(const Ref<WaterChunk>& chunk) { m_WaterChunk = chunk; }

		bool GetTerrainSurfacePointFromRay(const glm::vec3& ray, glm::vec3& out_point);

		void SetActive(bool active);
		const bool IsActive() const { return m_Active; }

		Entity FindClosestEntity(const glm::vec3& ray, const glm::vec3& ray_position);

		void ForEntityInScene(const std::function<void(Entity)>& callback);

	private:
		Scene(const std::string& name, const Ref<ICamera>& camera, const Ref<Terrain>& terrain, Light light);

		void RenderScene(const glm::vec4& ClipPlane = glm::vec4(0, -1, 0, 100000));

	private:
		bool m_Active = false;

		glm::ivec2 m_ViewportOffset;
		glm::ivec2 m_ViewportSize;

		std::string  m_Name;
		Ref<ICamera> m_Camera;
		Ref<Terrain> m_Terrain;
		Ref<Skybox>  m_Skybox;
		Light		 m_Light;
		Ref<WaterChunk> m_WaterChunk;

		std::map<Entity, std::vector<ComponentType>> m_EntityMap;

	private:
		std::vector<std::pair<Entity, short>> m_EntityRemovalPool;

		void CheckEntityRemovalPool();
		void RemoveEntityFromScene(Entity e);

	private:
		bool IsUnderGround(const glm::vec3& test_point);
		bool IsIntersectionInRange(float start, float finish, const glm::vec3& ray);
		glm::vec3 GetPointOnRay(const glm::vec3& ray, float distance);
		glm::vec3 BinarySearchRay(int count, float start, float finish, const glm::vec3& ray);

		static uint32_t RECURSION_COUNT;
		static uint32_t RAY_RANGE;
	};
}
