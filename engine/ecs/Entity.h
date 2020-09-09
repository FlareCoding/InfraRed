#pragma once
#include "IComponent.h"

namespace ifr
{
	typedef uint64_t Entity;

	constexpr Entity INVALID_ENTITY = 0;

	Entity CreateEntity();
	void DeleteEntity(Entity entity);

	IComponent* AddComponent(Entity e, ComponentType type);
	IComponent* GetComponent(Entity e, ComponentType type);
}
