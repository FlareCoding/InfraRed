#include "AssetManager.h"
#include "AssetLoadingQueue.h"
#include <map>

namespace ifr
{
	static Assets::fnAssetOnLoadCallback AssetOnLoadCallback = nullptr;

	static void AssetQueueOnLoadCallback(const std::string& name)
	{
		if (AssetOnLoadCallback)
			AssetOnLoadCallback(name);
	}

	static AssetLoadingQueue s_AssetLoadingQueue = AssetLoadingQueue(AssetQueueOnLoadCallback);

	static std::string MODELS_SOURCE_FOLDER = "assets\\models\\";
	static std::string TEXTURES_SOURCE_FOLDER = "assets\\textures\\";
	static std::string AUDIO_SOURCE_FOLDER = "assets\\audio\\";
	static std::string SCENES_SOURCE_FOLDER = "assets\\scenes\\";

#define MODELPATH(model) MODELS_SOURCE_FOLDER + model
#define TEXTUREPATH(texture) TEXTURES_SOURCE_FOLDER + texture
#define AUDIOPATH(audio) AUDIO_SOURCE_FOLDER + audio
#define SCENEEPATH(scene) SCENES_SOURCE_FOLDER + scene

#define ASSET_ON_LOAD(name) if (AssetOnLoadCallback) AssetOnLoadCallback(name);

	namespace Assets
	{
		void SetModelsSourceFolder(const std::string& path)
		{
			MODELS_SOURCE_FOLDER = path;
		}
		
		void SetTexturesSourceFolder(const std::string& path)
		{
			TEXTURES_SOURCE_FOLDER = path;
		}

		void SetAudioSourcesSourceFolder(const std::string& path)
		{
			AUDIO_SOURCE_FOLDER = path;
		}

		void SetScenesSourceFolder(const std::string& path)
		{
			SCENES_SOURCE_FOLDER = path;
		}

		void LoadTexture2D(const std::string& filename, const std::string& name, bool flip_on_load)
		{
			s_AssetLoadingQueue.LoadTexture2DAsset(TEXTUREPATH(filename), name, flip_on_load);
		}

		void LoadTextureCube(const std::vector<std::string>& filenames, const std::string& name)
		{
			std::vector<std::string> paths;
			for (auto& filename : filenames)
				paths.push_back(TEXTUREPATH(filename));

			s_AssetLoadingQueue.LoadTextureCubeAsset(paths, name);
		}

		void LoadStaticModel(const std::string& filename, const std::string& name, bool bShouldApplyCorrectionMatrix)
		{
			s_AssetLoadingQueue.LoadStaticModelAsset(MODELPATH(filename), name, bShouldApplyCorrectionMatrix);
		}

		void LoadAnimatedModel(const std::string& filename, const std::string& name, bool bShouldApplyCorrectionMatrix)
		{
			s_AssetLoadingQueue.LoadAnimatedModelAsset(MODELPATH(filename), name, bShouldApplyCorrectionMatrix);
		}

		void LoadAudioSource(const std::string& filename, const std::string& name)
		{
			s_AssetLoadingQueue.LoadAudioSourceAsset(AUDIOPATH(filename), name);
		}

		void LoadScene(const std::string& filename, const std::string& name)
		{
			s_AssetLoadingQueue.LoadSceneAsset(SCENEEPATH(filename), name);
		}

		Ref<Texture> FindTexture(const std::string& name)
		{
			return s_AssetLoadingQueue.RetrieveTextureAsset(name);
		}

		StaticModel FindStaticModel(const std::string& name)
		{
			return s_AssetLoadingQueue.RetrieveStaticModelAsset(name);
		}

		AnimatedModel FindAnimatedModel(const std::string& name)
		{
			return s_AssetLoadingQueue.RetrieveAnimatedModelAsset(name);
		}

		Ref<AudioSource> FindAudioSource(const std::string& name)
		{
			return s_AssetLoadingQueue.RetrieveAudioSourceAsset(name);
		}

		Ref<Scene> FindScene(const std::string& name)
		{
			return s_AssetLoadingQueue.RetrieveSceneAsset(name);
		}
	}
	
	void Assets::RegisterAssetOnLoadCallback(fnAssetOnLoadCallback callback)
	{
		AssetOnLoadCallback = callback;
	}
}
