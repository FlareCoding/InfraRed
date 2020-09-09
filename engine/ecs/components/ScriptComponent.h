#pragma once
#include "../IComponent.h"
#include <vector>

namespace ifr
{
	class UserScript
	{
	public:
		virtual void OnUpdate() = 0;
	};

#define TO_USER_SCRIPT(script_ref) std::static_pointer_cast<UserScript>(script_ref)

	class ScriptComponent : public IComponent
	{
	public:
		ScriptComponent() = default;

		// Inherited via IComponent
		virtual ComponentType GetType() override;
		virtual void OnAttach() {}
		virtual void OnUpdate() override;

		void AddScript(Ref<UserScript> script) { m_Scripts.push_back(script); }

	private:
		std::vector<Ref<UserScript>> m_Scripts;
	};
}

