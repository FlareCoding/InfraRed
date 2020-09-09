#pragma once
#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "Components.h"
#include <functional>
#include <mutex>

namespace ifr
{
	template <typename Component>
	class ComponentManager
	{
	public:
		bool HasComponentForEntity(Entity e);

		void AddComponent(Entity e);
		Component* GetComponent(Entity e);
		void RemoveComponent(Entity e);

		void ForEachComponent(std::function<void(Entity, Component&)> callback);

		uint32_t GetComponentCount();

	private:
		std::mutex table_mutex;
		std::unordered_map<Entity, Component> lookup_table;
	};

	template<typename Component>
	inline bool ComponentManager<Component>::HasComponentForEntity(Entity e)
	{
		table_mutex.lock();
		auto result = lookup_table.find(e) != lookup_table.end();
		table_mutex.unlock();

		return result;
	}

	template<typename Component>
	inline void ComponentManager<Component>::AddComponent(Entity e)
	{
		auto component = Component();
		component.Owner = e;

		table_mutex.lock();
		lookup_table[e] = component;
		lookup_table[e].OnAttach(); // use bracket operator to get component by reference
		table_mutex.unlock();
	}

	template<typename Component>
	inline Component* ComponentManager<Component>::GetComponent(Entity e)
	{
		if (HasComponentForEntity(e))
			return &lookup_table[e];
		else
			return nullptr;
	}

	template<typename Component>
	inline void ComponentManager<Component>::RemoveComponent(Entity e)
	{
		if (HasComponentForEntity(e))
		{
			table_mutex.lock();
			lookup_table.erase(e);
			table_mutex.unlock();
		}
	}

	template<typename Component>
	inline void ComponentManager<Component>::ForEachComponent(std::function<void(Entity, Component&)> callback)
	{
		for (auto& [entity, component] : lookup_table)
			callback(entity, component);
	}

	template<typename Component>
	inline uint32_t ComponentManager<Component>::GetComponentCount()
	{
		return (uint32_t)lookup_table.size();
	}

	extern ComponentManager<TransformComponent> TransformComponentManager;
	extern ComponentManager<StaticRenderComponent> StaticRenderComponentManager;
	extern ComponentManager<AnimatedRenderComponent> AnimatedRenderComponentManager;
	extern ComponentManager<ColliderComponent> ColliderComponentManager;
	extern ComponentManager<ScriptComponent> ScriptComponentManager;
	extern ComponentManager<InformationComponent> InformationComponentManager;
}
