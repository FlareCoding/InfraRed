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
	static std::string SCENES_SOURCE_FOLDER = "assets\\scenes\\";

#define MODELPATH(model) MODELS_SOURCE_FOLDER + model
#define TEXTUREPATH(texture) TEXTURES_SOURCE_FOLDER + texture
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

		void SetScenesSourceFolder(const std::string& path)
		{
			SCENES_SOURCE_FOLDER = path;
		}

		void LoadTexture2D(const std::string& filename, const std::string& name, bool flip_on_load)
		{
			s_AssetLoadingQueue.LoadTexture2DAsset(filename, name, flip_on_load);
		}

		void LoadTextureCube(const std::vector<std::string>& filenames, const std::string& name)
		{
			s_AssetLoadingQueue.LoadTextureCubeAsset(filenames, name);
		}

		void LoadStaticModel(const std::string& filename, const std::string& name, bool bShouldApplyCorrectionMatrix)
		{
			s_AssetLoadingQueue.LoadStaticModelAsset(filename, name, bShouldApplyCorrectionMatrix);
		}

		void LoadAnimatedModel(const std::string& filename, const std::string& name, bool bShouldApplyCorrectionMatrix)
		{
			s_AssetLoadingQueue.LoadAnimatedModelAsset(filename, name, bShouldApplyCorrectionMatrix);
		}

		void LoadScene(const std::string& filename, const std::string& name)
		{
			s_AssetLoadingQueue.LoadSceneAsset(filename, name);
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

		Ref<Scene> FindScene(const std::string& name)
		{
			return s_AssetLoadingQueue.RetrieveSceneAsset(name);
		}

		void LoadDefaultAssets()
		{	
			LoadTexture2D(TEXTUREPATH("Terrain.png"),				"TerrainTexture");
			LoadTexture2D(TEXTUREPATH("Heightmap1.png"),			"TerrainHeightmapTexture");
			LoadTexture2D(TEXTUREPATH("HighlandsBlendmap.png"),		"HighlandsBlendmapTexture");
			LoadTexture2D(TEXTUREPATH("HighlandsHeightmap.png"),	"HighlandsHeightmapTexture");
			LoadTexture2D(TEXTUREPATH("TownHeightmap.png"),			"TownHeightmapTexture");
			LoadTexture2D(TEXTUREPATH("TownBlendmap.png"),			"TownBlendmapTexture");
			LoadTexture2D(TEXTUREPATH("Sand.jpg"),					"SandTexture");
			LoadTexture2D(TEXTUREPATH("FlowersPath.png"),			"FlowersTexture");
			LoadTexture2D(TEXTUREPATH("StonePath.png"),				"StonePathTexture");

			LoadTextureCube(
				{ 
					TEXTUREPATH("skyboxes\\SB1_right.png"),
					TEXTUREPATH("skyboxes\\SB1_left.png"),
					TEXTUREPATH("skyboxes\\SB1_top.png"),
					TEXTUREPATH("skyboxes\\SB1_bottom.png"),
					TEXTUREPATH("skyboxes\\SB1_back.png"),
					TEXTUREPATH("skyboxes\\SB1_front.png")
				}, 
				"SkyboxSunny_Texture"
			);

			LoadTextureCube(
				{
					TEXTUREPATH("skyboxes\\SB2_right.jpg"),
					TEXTUREPATH("skyboxes\\SB2_left.jpg"),
					TEXTUREPATH("skyboxes\\SB2_top.jpg"),
					TEXTUREPATH("skyboxes\\SB2_bottom.jpg"),
					TEXTUREPATH("skyboxes\\SB2_back.jpg"),
					TEXTUREPATH("skyboxes\\SB2_front.jpg")
				},
				"SkyboxLake_Texture"
			);

			LoadTexture2D(TEXTUREPATH("Crate.png"),			"CrateTexture");
			LoadTexture2D(TEXTUREPATH("CrateNormal.png"),	"CrateNormalTexture");
			LoadStaticModel(MODELPATH("Crate.obj"),			"Crate", false);

			LoadTexture2D(TEXTUREPATH("Boulder.png"),		"BoulderTexture");
			LoadTexture2D(TEXTUREPATH("BoulderNormal.png"),	"BoulderNormalTexture");
			LoadStaticModel(MODELPATH("Boulder.obj"),		"Boulder", false);

			LoadTexture2D(TEXTUREPATH("Barrel.png"),		"BarrelTexture");
			LoadTexture2D(TEXTUREPATH("BarrelNormal.png"),	"BarrelNormalTexture");
			LoadStaticModel(MODELPATH("Barrel.obj"),		"Barrel", false);

			LoadStaticModel(MODELPATH("Ramp.obj"),			"Ramp", false);

			LoadTexture2D(TEXTUREPATH("WoodenCabin.jpg"),	"WoodenCabinTexture");
			LoadStaticModel(MODELPATH("WoodenCabin.obj"),	"WoodenCabin", false);

			LoadTexture2D(TEXTUREPATH("Cowboy.png"),		"CowboyTexture");
			LoadAnimatedModel(MODELPATH("Cowboy.fbx"),		"Cowboy", true);

			LoadTexture2D(TEXTUREPATH("Truck.png"),			"TruckTexture");
			LoadStaticModel(MODELPATH("Truck.fbx"), 		"Truck", false);

			LoadTexture2D(TEXTUREPATH("Pistol.png"),		"PistolTexture");
			LoadAnimatedModel(MODELPATH("Pistol.fbx"),		"Pistol", false);

			LoadTexture2D(TEXTUREPATH("Lumberjack.png"),	"LumberjackTexture");
			LoadAnimatedModel(MODELPATH("Lumberjack.fbx"),	"Lumberjack", true);

			LoadStaticModel(MODELPATH("Sphere.obj"),		"Sphere", false);
			LoadStaticModel(MODELPATH("Cylinder.obj"),		"Cylinder", false);
			LoadStaticModel(MODELPATH("Cube.obj"),			"Cube", false);

			LoadTexture2D(TEXTUREPATH("AbandonedBuilding.jpg"), "AbandonedBuildingTexture");
			LoadStaticModel(MODELPATH("AbandonedBuilding.obj"), "AbandonedBuilding", false);

			LoadScene(SCENEEPATH("Scene1.ifrscene"),		"Scene_Scene1");
		}
	}
	
	void Assets::RegisterAssetOnLoadCallback(fnAssetOnLoadCallback callback)
	{
		AssetOnLoadCallback = callback;
	}
}
