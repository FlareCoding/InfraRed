#pragma once
#include <scene/Scene.h>

namespace ifr
{
	class IFRSceneLoader
	{
	public:
		static Ref<Scene> Import(const std::string& filepath);
		static void Export(Ref<Scene>& scene, const std::string& filepath);
	};
}
