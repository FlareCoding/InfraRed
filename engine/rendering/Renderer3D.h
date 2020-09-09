#pragma once
#include <math/IFRMath.h>
#include <rendering/shaders/Shader.h>
#include <camera/ICamera.h>
#include "Material.h"
#include <model_data/shapes/Shapes.h>

namespace ifr
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(ICamera& camera, const Light& light);
		static void EndScene();

		static void DrawCube(const glm::vec3& position, float size, const glm::vec3& color);

		static void DrawMesh(
			const Ref<Mesh>& mesh, 
			const glm::vec3& position, 
			const glm::vec3& rotation, 
			Ref<Texture> texture, 
			const glm::vec3& color = { 1, 1, 1 }, 
			float opacity = 1.0f, 
			glm::vec3 scale = glm::vec3(1, 1, 1)
		);

		// In case custom shader is provided and being used
		static void DrawRawMeshData(const Ref<Mesh>& mesh);
		static void DrawRawVertexArrayData(const Ref<VertexArray>& va);
		static void DrawRawVertexArrayDataInstanced(const Ref<VertexArray>& va, uint32_t instance_count);
		static void DrawRawMeshDataControlPointPatchList(const Ref<Mesh>& mesh);
		
		static void DrawMesh(
			const Ref<Mesh>& mesh,
			const glm::vec3& position,
			const glm::vec3& rotation,
			const glm::vec3& scale,
			const Ref<MaterialInstance>& material,
			const glm::vec4& clip_plane = glm::vec4(0, -1, 0, 100000)
		);

		static void DrawAnimatedMesh(
			const Ref<Mesh>& mesh,
			std::vector<glm::mat4>& bone_transforms,
			const glm::vec3& position,
			const glm::vec3& rotation,
			const glm::vec3& scale,
			const Ref<MaterialInstance>& material,
			const glm::vec4& clip_plane = glm::vec4(0, -1, 0, 100000)
		);

		static void DrawAnimatedMesh(
			const Ref<Mesh>& mesh,
			std::vector<glm::mat4>& bone_transforms,
			const glm::vec3& position,
			const glm::vec3& rotation,
			Ref<Texture> texture,
			glm::vec3 color = { 1, 1, 1 },
			float opacity = 1.0f,
			glm::vec3 scale = glm::vec3(1, 1, 1)
		);

		static void BindBasicShader();
		static void BindBasicAnimShader();

	private:
		struct SceneData
		{
			glm::mat4 ProjectionMatrix;
			glm::mat4 ViewMatrix;
			Light light;
			glm::vec3 CameraPosition;
		};

		static SceneData m_SceneData;

	private:
		static Ref<Mesh> m_Cube;
		static Ref<BasicShader> m_Shader;
		static Ref<BasicAnimShader> m_AnimShader;
		static Ref<Texture> m_WhiteTexture;
	};
}
