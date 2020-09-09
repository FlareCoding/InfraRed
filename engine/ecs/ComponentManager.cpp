#include "ComponentManager.h"

namespace ifr
{
	ComponentManager<TransformComponent> TransformComponentManager;
	ComponentManager<StaticRenderComponent> StaticRenderComponentManager;
	ComponentManager<AnimatedRenderComponent> AnimatedRenderComponentManager;
	ComponentManager<ColliderComponent> ColliderComponentManager;
	ComponentManager<ScriptComponent> ScriptComponentManager;
	ComponentManager<InformationComponent> InformationComponentManager;
}
