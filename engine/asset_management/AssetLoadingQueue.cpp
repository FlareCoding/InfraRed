#include "AssetLoadingQueue.h"
#include <mutex>

namespace ifr 
{
	static std::mutex asset_queue_mutex;

	AssetLoadingQueue::AssetLoadingQueue(std::function<void(const std::string&)> on_load_callback) : m_OnLoadCallback(on_load_callback)
	{
		std::thread asset_queue_thread = std::thread(&AssetLoadingQueue::AssetLoadingLoop, this);
		asset_queue_thread.detach();
	}

	void AssetLoadingQueue::LoadTexture2DAsset(const std::string& filepath, const std::string& name, bool flip_on_load)
	{
		AssetInfo info;
		info.Filepath = filepath;
		info.Name = name;
		info.Type = AssetType::Texture2D;
		info.AdditionalInfo = flip_on_load;

		AddAssetInfoToQueue(info);
	}

	void AssetLoadingQueue::LoadTextureCubeAsset(const std::vector<std::string>& filepaths, const std::string& name)
	{
		AssetInfo info;
		info.MultipleFilepaths = filepaths;
		info.Name = name;
		info.Type = AssetType::TextureCube;

		AddAssetInfoToQueue(info);
	}

	void AssetLoadingQueue::LoadStaticModelAsset(const std::string& filepath, const std::string& name, bool bApplyCorrectionMatrix)
	{
		AssetInfo info;
		info.Filepath = filepath;
		info.Name = name;
		info.Type = AssetType::StaticModel;
		info.AdditionalInfo = bApplyCorrectionMatrix;

		AddAssetInfoToQueue(info);
	}

	void AssetLoadingQueue::LoadAnimatedModelAsset(const std::string& filepath, const std::string& name, bool bApplyCorrectionMatrix)
	{
		AssetInfo info;
		info.Filepath = filepath;
		info.Name = name;
		info.Type = AssetType::AnimatedModel;
		info.AdditionalInfo = bApplyCorrectionMatrix;

		AddAssetInfoToQueue(info);
	}

	void AssetLoadingQueue::LoadAudioSourceAsset(const std::string& filepath, const std::string& name)
	{
		AssetInfo info;
		info.Filepath = filepath;
		info.Name = name;
		info.Type = AssetType::AudioSource;

		AddAssetInfoToQueue(info);
	}

	void AssetLoadingQueue::LoadSceneAsset(const std::string& filepath, const std::string& name)
	{
		AssetInfo info;
		info.Filepath = filepath;
		info.Name = name;
		info.Type = AssetType::Scene;

		AddAssetInfoToQueue(info);
	}

	Ref<Texture> AssetLoadingQueue::RetrieveTextureAsset(const std::string& name)
	{
		if (m_CustomLoadedTextures.find(name) != m_CustomLoadedTextures.end())
			return m_CustomLoadedTextures[name];
		else
			return nullptr;
	}

	StaticModel AssetLoadingQueue::RetrieveStaticModelAsset(const std::string& name)
	{
		if (m_CustomLoadedStaticModels.find(name) != m_CustomLoadedStaticModels.end())
			return m_CustomLoadedStaticModels[name];
		else
			return StaticModel();
	}

	AnimatedModel AssetLoadingQueue::RetrieveAnimatedModelAsset(const std::string& name)
	{
		if (m_CustomLoadedAnimatedModels.find(name) != m_CustomLoadedAnimatedModels.end())
			return m_CustomLoadedAnimatedModels[name];
		else
			return AnimatedModel();
	}

	Ref<AudioSource> AssetLoadingQueue::RetrieveAudioSourceAsset(const std::string& name)
	{
		if (m_CustomLoadedAudioSources.find(name) != m_CustomLoadedAudioSources.end())
			return m_CustomLoadedAudioSources[name];
		else
			return nullptr;
	}

	Ref<Scene> AssetLoadingQueue::RetrieveSceneAsset(const std::string& name)
	{
		if (m_CustomLoadedScenes.find(name) != m_CustomLoadedScenes.end())
			return m_CustomLoadedScenes[name];
		else
			return nullptr;
	}

	void AssetLoadingQueue::AssetLoadingLoop()
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(400));

			asset_queue_mutex.lock();
			try
			{
				for (auto& asset : m_AssetQueue)
				{
					switch (asset.Type)
					{
					case AssetType::Texture2D:
					{
						m_CustomLoadedTextures[asset.Name] = Texture2D::LoadFromFile(asset.Filepath, TextureFormat::RGBA8, asset.AdditionalInfo);
						m_CustomLoadedTextures[asset.Name]->Name = asset.Name;
						m_OnLoadCallback(asset.Name);
						break;
					}
					case AssetType::TextureCube:
					{
						m_CustomLoadedTextures[asset.Name] = Texture3D::LoadFromFiles(asset.MultipleFilepaths);
						m_CustomLoadedTextures[asset.Name]->Name = asset.Name;
						m_OnLoadCallback(asset.Name);
						break;
					}
					case AssetType::StaticModel:
					{
						m_CustomLoadedStaticModels[asset.Name] = AssimpLoader::Load(asset.Filepath, asset.AdditionalInfo);
						m_CustomLoadedStaticModels[asset.Name].Name = asset.Name;
						m_OnLoadCallback(asset.Name);
						break;
					}
					case AssetType::AnimatedModel:
					{
						m_CustomLoadedAnimatedModels[asset.Name] = AssimpLoader::LoadAnimated(asset.Filepath, asset.AdditionalInfo);
						m_CustomLoadedAnimatedModels[asset.Name].Name = asset.Name;
						m_OnLoadCallback(asset.Name);
						break;
					}
					case AssetType::AudioSource:
					{
						m_CustomLoadedAudioSources[asset.Name] = AudioEngine::Get().LoadAndRegisterWAVAudioSource(asset.Filepath, asset.Name);
						m_OnLoadCallback(asset.Name);
						break;
					}
					case AssetType::Scene:
					{
						m_CustomLoadedScenes[asset.Name] = IFRSceneLoader::Import(asset.Filepath);
						m_OnLoadCallback(asset.Name);
						break;
					}
					default:
						break;
					}
				}

				m_AssetQueue.clear();
			}
			catch (...) {}
			asset_queue_mutex.unlock();
		}
	}
	
	void AssetLoadingQueue::AddAssetInfoToQueue(AssetInfo info)
	{
		m_AssetQueue.push_back(info);
	}
}
