#pragma once
#include "loaders/assimp/AssimpLoader.h"
#include "loaders/scene/IFRSceneLoader.h"
#include <string>
#include <functional>

namespace ifr
{
	namespace Assets
	{
		using fnAssetOnLoadCallback = std::function<void(const std::string & name)>;
		void RegisterAssetOnLoadCallback(fnAssetOnLoadCallback callback);

		void LoadDefaultAssets();

		void SetModelsSourceFolder(const std::string& path);
		void SetTexturesSourceFolder(const std::string& path);
		void SetScenesSourceFolder(const std::string& path);

		void LoadTexture2D(const std::string& filename, const std::string& name, bool flip_on_load = true);
		void LoadTextureCube(const std::vector<std::string>& filenames, const std::string& name);
		void LoadStaticModel(const std::string& filename, const std::string& name, bool bShouldApplyCorrectionMatrix);
		void LoadAnimatedModel(const std::string& filename, const std::string& name, bool bShouldApplyCorrectionMatrix);
		void LoadScene(const std::string& filename, const std::string& name);

		Ref<Texture>	FindTexture(const std::string& name);
		StaticModel	FindStaticModel(const std::string& name);
		AnimatedModel FindAnimatedModel(const std::string& name);
		Ref<Scene>	FindScene(const std::string& name);
	}
}
