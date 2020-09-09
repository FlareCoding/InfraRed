#include "LevelManager.h"

namespace ifr
{
	LevelManager* LevelManager::s_Instance = nullptr;

	LevelManager::LevelManager()
	{
		s_Instance = this;
	}

	LevelManager& LevelManager::Get()
	{
		return *s_Instance;
	}
	
	void LevelManager::RegisterLevel(Ref<Level> level, const std::string& name, bool make_active)
	{
		if (m_LevelMap.find(name) == m_LevelMap.end())
		{
			m_LevelMap[name] = level;

			level->OnCreate();

			if (make_active)
				m_ActiveLevel = level;
		}
	}

	Ref<Level> LevelManager::GetLevel(const std::string& name)
	{
		if (m_LevelMap.find(name) != m_LevelMap.end())
			return m_LevelMap[name];
		else
			return nullptr;
	}
	
	void LevelManager::SetActiveLevel(const std::string& name)
	{
		if (m_LevelMap.find(name) != m_LevelMap.end())
			SetActiveLevel(m_LevelMap[name]);
	}
	
	void LevelManager::SetActiveLevel(Ref<Level> level)
	{
		m_ActiveLevel = level;
		m_ActiveLevel->OnBecameActive();
	}

	void LevelManager::OnRender()
	{
		if (m_ActiveLevel)
			m_ActiveLevel->OnRender();
	}
	
	void LevelManager::OnEvent(Event& e)
	{
		if (m_ActiveLevel)
			m_ActiveLevel->OnEvent(e);
	}
}
