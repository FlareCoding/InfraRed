#include "Terrain.h"
#include <rendering/Renderer3D.h>
#include <algorithm>

constexpr uint64_t MAX_PIXEL_COLOR_RGBA8 = 256 * 256 * 256;
constexpr uint64_t MAX_PIXEL_COLOR_RGBA16 = (uint64_t)65536  * (uint64_t)65536  * (uint64_t)65536;

namespace ifr
{
	Ref<Terrain> Terrain::Create(uint32_t size, uint32_t vertex_count, Ref<Texture> heightmap, float max_height)
	{
		return Ref<Terrain>(new Terrain(size, vertex_count, heightmap, max_height));
	}

	Terrain::Terrain(uint32_t size, uint32_t vertex_count, Ref<Texture> heightmap, float max_height) : size(size), vcount(vertex_count)
	{
		GenerateMesh(heightmap, max_height);

		m_Shader = Shader::CreateDefaultTerrainShader();
		m_DefaultTexture = Texture2D::CreateFromColor({ 100, 160, 120 });

		if (heightmap) m_HeightmapRef = heightmap;
	}

	void Terrain::GenerateMesh(Ref<Texture> heightmap, float max_height)
	{
		MeshData data;

		if (heightmap != nullptr)
			vcount = heightmap->GetHeight();

		for (uint32_t i = 0; i < vcount; i++)
		{
			//std::vector<float> heights_row;

			for (uint32_t j = 0; j < vcount; j++)
			{
				Vertex vertex;

				float height = heightmap ? GetHeightFromHeightmap(j, i, heightmap, max_height) : 0;

				vertex.position.x = (float)j / ((float)vcount - 1) * size;
				vertex.position.y = height;
				vertex.position.z = (float)i / ((float)vcount - 1) * size;

				vertex.uv.x = (float)j / ((float)vcount - 1);
				vertex.uv.y = (float)i / ((float)vcount - 1);

				vertex.normal = heightmap ? CalculateNormal(j, i, heightmap, max_height) : glm::vec3(0, 1, 0);

				data.Vertices.push_back(vertex);

				//heights_row.push_back(height);
			}

			//m_VertexHeights.push_back(heights_row);
		}

		for (uint32_t gz = 0; gz < vcount - 1; gz++)
		{
			for (uint32_t gx = 0; gx < vcount - 1; gx++)
			{
				auto top_left = (gz * vcount) + gx;
				auto top_right = top_left + 1;
				auto bottom_left = ((gz + 1) * vcount) + gx;
				auto bottom_right = bottom_left + 1;

				data.Indices.push_back(top_left);
				data.Indices.push_back(bottom_left);
				data.Indices.push_back(top_right);
				data.Indices.push_back(top_right);
				data.Indices.push_back(bottom_left);
				data.Indices.push_back(bottom_right);
			}
		}

		mesh = Mesh::Create(data);
	}

	float Terrain::GetHeightFromHeightmap(uint32_t x, uint32_t z, Ref<Texture> heightmap, float max_height)
	{
		if (x < 0 || x >= heightmap->GetHeight() || z < 0 || z >= heightmap->GetHeight())
			return 0;

		auto pixel = heightmap->ReadPixel(x, z);

		uint64_t rgb = (
			((pixel.a & 0xff) << 24) |
			((pixel.r & 0xff) << 16) |
			((pixel.g & 0xff) << 8) |
			((pixel.b & 0xff) << 0)
		);

		double height = (double)rgb;

		auto max_pixel_color = (heightmap->Format == TextureFormat::RGBA16) ? MAX_PIXEL_COLOR_RGBA16 : MAX_PIXEL_COLOR_RGBA8;

		height += ((double)max_pixel_color / 2.0f);
		height /= (double)max_pixel_color / 2.0f;
		height *= max_height;

		return (float)height;
	}

	glm::vec3 Terrain::CalculateNormal(int x, int z, Ref<Texture> heightmap, float max_height)
	{
		float heightL = GetHeightFromHeightmap(x - 1, z, heightmap, max_height);
		float heightR = GetHeightFromHeightmap(x + 1, z, heightmap, max_height);
		float heightD = GetHeightFromHeightmap(x, z - 1, heightmap, max_height);
		float heightU = GetHeightFromHeightmap(x, z + 1, heightmap, max_height);

		return glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));
	}

	glm::vec3 Terrain::CalculateVertexNormal(float worldX, float worldZ)
	{
		float heightL = mesh->GetMeshData().Vertices[GetVertexIndex(worldX - 1, worldZ)].position.y;
		float heightR = mesh->GetMeshData().Vertices[GetVertexIndex(worldX + 1, worldZ)].position.y;
		float heightD = mesh->GetMeshData().Vertices[GetVertexIndex(worldX, worldZ - 1)].position.y;
		float heightU = mesh->GetMeshData().Vertices[GetVertexIndex(worldX, worldZ + 1)].position.y;

		return glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));
	}

	glm::mat4 Terrain::GetTransformationMatrix()
	{
		auto translation = glm::vec3(position.x * size, 0, position.y * size);
		auto scaling_matrix = glm::scale(glm::mat4(1.0f), { 1, 1, 1 });

		return glm::translate(glm::mat4(1.0f), translation) * scaling_matrix;
	}

	float Terrain::GetSurfaceHeight(float x, float z)
	{
		float terrainX = x - (position.x * size);
		float terrainZ = z - (position.y * size);

		float grid_square_size = (float)size / ((float)m_VertexHeights.size() - 1);

		int gridX = (int)(floorf(terrainX / grid_square_size));
		int gridZ = (int)(floorf(terrainZ / grid_square_size));

		if (gridX >= (int)m_VertexHeights.size() - 1 || gridZ >= (int)m_VertexHeights.size() - 1 || gridX < 0 || gridZ < 0)
			return 0;

		float xCoord = fmodf(terrainX, grid_square_size) / grid_square_size;
		float zCoord = fmodf(terrainZ, grid_square_size) / grid_square_size;

		if (xCoord <= (1 - zCoord))
		{
			return barry_centric(glm::vec3(0, m_VertexHeights[gridZ][gridX], 0), glm::vec3(1, m_VertexHeights[(gridZ + 1)][gridX], 0), glm::vec3(0, m_VertexHeights[gridZ][gridX + 1], 1), glm::vec2(zCoord, xCoord));
		}
		else
		{
			return barry_centric(glm::vec3(1, m_VertexHeights[gridZ + 1][gridX], 0), glm::vec3(1, m_VertexHeights[gridZ + 1][gridX + 1], 1), glm::vec3(0, m_VertexHeights[gridZ][gridX + 1], 1), glm::vec2(zCoord, xCoord));
		}
	}

	std::vector<size_t> Terrain::GetVertexIndicesInRange(const glm::vec2& point, float range)
	{
		float PlaneSpaceX = point.x - (position.x * size);
		float PlaneSpaceZ = point.y - (position.y * size);

		float GridSquareSize = (float)size / ((float)m_VertexHeights.size() - 1.0f);

		float LeftBound		= PlaneSpaceX - range;
		float RightBound	= PlaneSpaceX + range;
		float TopBound		= PlaneSpaceZ - range;
		float BottomBound	= PlaneSpaceZ + range;

		uint32_t LeftBoundIdx	= (uint32_t)roundf(LeftBound	/ GridSquareSize);
		uint32_t RightBoundIdx	= (uint32_t)roundf(RightBound	/ GridSquareSize);
		uint32_t TopBoundIdx	= (uint32_t)roundf(TopBound		/ GridSquareSize);
		uint32_t BottomBoundIdx	= (uint32_t)roundf(BottomBound	/ GridSquareSize);

		LeftBoundIdx	= CLAMP(LeftBoundIdx,	0U, vcount - 1);
		RightBoundIdx	= CLAMP(RightBoundIdx,	0U, vcount - 1);
		TopBoundIdx		= CLAMP(TopBoundIdx,	0U, vcount - 1);
		BottomBoundIdx	= CLAMP(BottomBoundIdx, 0U, vcount - 1);

		std::vector<size_t> indices;

		for (auto i = LeftBoundIdx; i < RightBoundIdx; i++)
			for (auto j = TopBoundIdx; j < BottomBoundIdx; j++)
				indices.push_back(((size_t)j * (size_t)vcount) + (size_t)i);

		return indices;
	}

	size_t Terrain::GetVertexIndex(float worldX, float worldZ)
	{
		float GridSquareSize = (float)size / ((float)m_VertexHeights.size() - 1.0f);

		size_t x_index = (size_t)(worldX / GridSquareSize);
		size_t z_index = (size_t)(worldZ / GridSquareSize);

		return z_index * (size_t) + x_index;
	}

	void Terrain::Render(ICamera& camera, const Light& light)
	{
		if (PrimaryTexture)
			PrimaryTexture->Bind(0);
		else
			m_DefaultTexture->Bind(0);

		if (SupportTexture1) SupportTexture1->Bind(1);
		if (SupportTexture2) SupportTexture2->Bind(2);
		if (SupportTexture3) SupportTexture3->Bind(3);
		if (Blendmap) Blendmap->Bind(4);

		if (Heightmap) Heightmap->Bind(0);

		m_Shader->Bind();
		m_Shader->FillUniformBuffers(
			GetTransformationMatrix(), 
			camera.GetProjection(), 
			camera.GetViewMatrix(), 
			light, 
			camera.GetPosition(),
			PrimaryTextureTiling,
			SupportTexture1Tiling,
			SupportTexture2Tiling,
			SupportTexture3Tiling,
			BrushCenter,
			BrushRadius
		);

		Renderer3D::DrawRawMeshDataControlPointPatchList(mesh);

		m_Shader->Unbind();
	}
}
