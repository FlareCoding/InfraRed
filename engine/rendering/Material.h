#pragma once
#include <rendering/shader_graph/ShaderGraph.h>
#include <rendering/shaders/Shader.h>
#include "Texture.h"
#include <scene/Light.h>
#include <camera/ICamera.h>

namespace ifr
{
	class Material
	{
		friend class MaterialInstance;

	public:
		static Ref<Material> Create(ShaderGraph& graph);

	private:
		Material(ShaderGraph& graph);

	private:
		ShaderGraph m_ShaderGraph;
	};

	class MaterialInstance
	{
	public:
		static Ref<MaterialInstance> Create(const Ref<Material>& material, ShaderGraphMeshType mesh_type = MESH_STATIC);

		template <typename T>
		void SetUniform(const char* name, const T& value);

		template <typename T>
		T GetUniformValue(const char* name, const T& default_val = T());

		void SetRoughness(float value);
		float GetRoughness() const { return m_Roughness; }

		void Bind();
		void Unbind();

		void AddTexture(const Ref<Texture>& texture);
		Ref<Texture> GetTexture(size_t idx);
		uint32_t GetTextureCount() const { return (uint32_t)m_Textures.size(); }

		void SetObjectSceneProperties(
			const glm::mat4& transform, 
			const glm::mat4& projection, 
			const glm::mat4& view, 
			const glm::vec3& camera_pos, 
			Light& light, 
			const glm::vec4& clip_plane = glm::vec4(0, -1, 0, 100000)
		);

		void SetObjectSceneProperties(
			const glm::mat4& transform, 
			const glm::mat4& projection, 
			const glm::mat4& view, 
			const glm::vec3& camera_pos, 
			Light& light, 
			std::vector<glm::mat4>& bone_transforms, 
			const glm::vec4& clip_plane = glm::vec4(0, -1, 0, 100000)
		);

	private:
		MaterialInstance(const Ref<Material>& material, ShaderGraphMeshType mesh_type);
		void CreateShader();

		ShaderGraphMeshType m_MeshType;

		Ref<Shader> m_Shader = nullptr;

		Ref<UniformBuffer> m_MVPBuffer		= nullptr;
		Ref<UniformBuffer> m_VSBuffer		= nullptr;
		Ref<UniformBuffer> m_PSBuffer		= nullptr;
		Ref<UniformBuffer> m_MaterialBuffer = nullptr;
		Ref<UniformBuffer> m_BoneTransformsBuffer = nullptr;

	private:
		float m_Roughness = -1.0f;

	private:
		Ref<Material> m_MaterialRef = nullptr;
		std::vector<Ref<Texture>> m_Textures;
	};

	template <typename T>
	void MaterialInstance::SetUniform(const char* name, const T& value)
	{
		T intermidiate = value;
		m_MaterialBuffer->SetVariableValue(name, reinterpret_cast<void*>(&intermidiate));
	}

	template<typename T>
	inline T MaterialInstance::GetUniformValue(const char* name, const T& default_val)
	{
		T val = default_val;
		m_MaterialBuffer->GetVariableValue(name, &val);
		return val;
	}
}
