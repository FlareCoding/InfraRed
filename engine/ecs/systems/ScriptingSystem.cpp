#include "ScriptingSystem.h"
#include "../ComponentManager.h"

namespace ifr
{
	void ScriptingSystem::Update()
	{
		ScriptComponentManager.ForEachComponent([](Entity entity, ScriptComponent& component) {
			if (!component.Enabled) return;

			component.OnUpdate();
		});
	}
}
