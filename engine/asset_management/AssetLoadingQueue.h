#pragma once
#include "AssetManager.h"
#include <thread>

namespace ifr
{
	enum class AssetType
	{
		Texture2D,
		TextureCube,
		StaticModel,
		AnimatedModel,
		Scene
	};

	struct AssetInfo
	{
		std::string Filepath;
		std::vector<std::string> MultipleFilepaths;
		std::string Name;
		AssetType Type;
		int AdditionalInfo;
	};

	class AssetLoadingQueue
	{
	public:
		AssetLoadingQueue(std::function<void(const std::string&)> on_load_callback);

		void LoadTexture2DAsset(const std::string& filepath, const std::string& name, bool flip_on_load = true);
		void LoadTextureCubeAsset(const std::vector<std::string>& filepaths, const std::string& name);
		void LoadStaticModelAsset(const std::string& filepath, const std::string& name, bool bApplyCorrectionMatrix);
		void LoadAnimatedModelAsset(const std::string& filepath, const std::string& name, bool bApplyCorrectionMatrix);
		void LoadSceneAsset(const std::string& filepath, const std::string& name);

		Ref<Texture>	RetrieveTextureAsset(const std::string& name);
		StaticModel		RetrieveStaticModelAsset(const std::string& name);
		AnimatedModel	RetrieveAnimatedModelAsset(const std::string& name);
		Ref<Scene>		RetrieveSceneAsset(const std::string& name);

	private:
		std::vector<AssetInfo> m_AssetQueue;
		void AssetLoadingLoop();
		void AddAssetInfoToQueue(AssetInfo info);

		std::map<std::string, Ref<Texture>>	 m_CustomLoadedTextures;
		std::map<std::string, StaticModel>	 m_CustomLoadedStaticModels;
		std::map<std::string, AnimatedModel> m_CustomLoadedAnimatedModels;
		std::map<std::string, Ref<Scene>>	 m_CustomLoadedScenes;

		std::function<void(const std::string&)> m_OnLoadCallback;
	};
}
