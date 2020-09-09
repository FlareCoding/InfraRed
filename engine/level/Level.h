#pragma once
#include <scene/Scene.h>

namespace ifr
{
	class Level
	{
	public:
		Ref<Scene>	LevelScene	= nullptr;

		virtual void OnCreate() = 0;
		virtual void OnEvent(Event& e) {}
		virtual void OnBecameActive() {}
		virtual void OnRender() = 0;
	};
}
