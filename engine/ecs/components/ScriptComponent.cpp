#include "ScriptComponent.h"

namespace ifr
{
	ComponentType ScriptComponent::GetType()
	{
		return ComponentType::Script;
	}
	
	void ScriptComponent::OnUpdate()
	{
		for (auto& script : m_Scripts)
			script->OnUpdate();
	}
}
