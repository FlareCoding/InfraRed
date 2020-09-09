#pragma once
#include "Level.h"

namespace ifr
{
	class LevelManager
	{
		friend class IFRApplication;

	public:
		static LevelManager& Get();

		void RegisterLevel(Ref<Level> level, const std::string& name = "Level", bool make_active = false);
		Ref<Level> GetLevel(const std::string& name);

		void SetActiveLevel(const std::string& name);
		void SetActiveLevel(Ref<Level> level);

	private:
		static LevelManager* s_Instance;

		Ref<Level> m_ActiveLevel = nullptr;
		std::map <std::string, Ref<Level>> m_LevelMap;

	private:
		LevelManager();

		void OnRender();
		void OnEvent(Event& e);
	};
}