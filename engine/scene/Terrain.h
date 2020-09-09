#pragma once
#include <model_data/StaticModel.h>
#include <camera/ICamera.h>
#include <rendering/shaders/Shaders.h>

namespace ifr
{
	class Terrain
	{
	public:
		static Ref<Terrain> Create(uint32_t size, uint32_t vertex_count = 512, Ref<Texture> heightmap = nullptr, float max_height = 40.0f);

		const uint32_t size = 0;
		uint32_t vcount = 0;

		glm::vec2 position = glm::vec2(0, 0);
		Ref<Mesh> mesh = nullptr;

		// Used in the terrain editor
		glm::vec3 BrushCenter = glm::vec3(0, 0, 0);
		float BrushRadius = 0.0f;

		Ref<Texture> PrimaryTexture = nullptr;
		Ref<Texture> SupportTexture1 = nullptr;
		Ref<Texture> SupportTexture2 = nullptr;
		Ref<Texture> SupportTexture3 = nullptr;
		Ref<Texture> Blendmap = nullptr;
		Ref<Texture> Heightmap = nullptr;

		float PrimaryTextureTiling = 1;
		float SupportTexture1Tiling = 1;
		float SupportTexture2Tiling = 1;
		float SupportTexture3Tiling = 1;

		float LODLevel0 = 64.0f;
		float LODLevel1 = 32.0f;
		float LODLevel2 = 16.0f;
		float LODLevel3 = 8.0f;
		float LODLevel4 = 4.0f;

		glm::mat4 GetTransformationMatrix();
		float GetSurfaceHeight(float x, float z);

		size_t GetVertexIndex(float worldX, float worldZ);
		glm::vec3 CalculateVertexNormal(float worldX, float worldZ);

		std::vector<size_t> GetVertexIndicesInRange(const glm::vec2& point, float range);

		void Render(ICamera& camera, const Light& light);

		const Ref<Texture> GetHeightmapRef() { return m_HeightmapRef; }

	private:
		Terrain(uint32_t size, uint32_t vertex_count, Ref<Texture> heightmap, float max_height);

		Ref<TerrainShader> m_Shader;
		Ref<Texture> m_DefaultTexture;
		Ref<Texture> m_HeightmapRef = nullptr;

		std::vector<std::vector<float>> m_VertexHeights;

	private:
		void GenerateMesh(Ref<Texture> heightmap, float max_height);
		float GetHeightFromHeightmap(uint32_t x, uint32_t z, Ref<Texture> heightmap, float max_height);
		glm::vec3 CalculateNormal(int x, int z, Ref<Texture> heightmap, float max_height);
	};
}
