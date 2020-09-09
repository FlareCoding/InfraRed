#include "Entity.h"
#include "ComponentManager.h"

namespace ifr
{
#define ADD_COMPONENT_TO_MANAGER(entity, component_type)    case ComponentType::##component_type: \
															{ \
																component_type##ComponentManager.AddComponent(entity); \
																return component_type##ComponentManager.GetComponent(entity); \
															}

#define GET_COMPONENT_FROM_MANAGER(entity, component_type)  case ComponentType::##component_type: \
															{ \
																return component_type##ComponentManager.GetComponent(entity); \
															}

#define REMOVE_COMPONENT_REFERENCE(entity, component_type) component_type##ComponentManager.RemoveComponent(entity);

	Entity CreateEntity()
	{
		static Entity available_entity_id = 0;
		Entity new_entity = ++available_entity_id;

		// Because all entities need a transform component,
		// the engine should add it automatically to all entities
		AddComponent(new_entity, ComponentType::Transform);

		return new_entity;
	}

	void DeleteEntity(Entity e)
	{
		REMOVE_COMPONENT_REFERENCE(e, Transform);
		REMOVE_COMPONENT_REFERENCE(e, StaticRender);
		REMOVE_COMPONENT_REFERENCE(e, AnimatedRender);
		REMOVE_COMPONENT_REFERENCE(e, Collider);
		REMOVE_COMPONENT_REFERENCE(e, Script);
		REMOVE_COMPONENT_REFERENCE(e, Information);
	}
	
	IComponent* AddComponent(Entity e, ComponentType type)
	{
		switch (type)
		{
		ADD_COMPONENT_TO_MANAGER(e, Transform)
		ADD_COMPONENT_TO_MANAGER(e, StaticRender)
		ADD_COMPONENT_TO_MANAGER(e, AnimatedRender)
		ADD_COMPONENT_TO_MANAGER(e, Collider)
		ADD_COMPONENT_TO_MANAGER(e, Script)
		ADD_COMPONENT_TO_MANAGER(e, Information)

		default:
			break;
		}

		return nullptr;
	}
	
	IComponent* GetComponent(Entity e, ComponentType type)
	{
		switch (type)
		{
		GET_COMPONENT_FROM_MANAGER(e, Transform)
		GET_COMPONENT_FROM_MANAGER(e, StaticRender)
		GET_COMPONENT_FROM_MANAGER(e, AnimatedRender)
		GET_COMPONENT_FROM_MANAGER(e, Collider)
		GET_COMPONENT_FROM_MANAGER(e, Script)
		GET_COMPONENT_FROM_MANAGER(e, Information)

		default:
			break;
		}

		return nullptr;
	}
}
