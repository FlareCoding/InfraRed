#pragma once
#include "Shader.h"
#include <scene/Light.h>

namespace ifr
{
	class BasicShader : public Shader
	{
	public:
		static Ref<BasicShader> Create();

		void SetMVPBuffer(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view);
		void SetBasicShaderBuffer(const glm::vec3& color, float opacity, float texture_tiling = 1.0f);
		void SetSceneBuffers(const Light& light, const glm::vec3& camera_pos);

	private:
		BasicShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers);

	private:
		Ref<UniformBuffer> m_MVPBuffer;
		Ref<UniformBuffer> m_BasicShaderBuffer;
		Ref<UniformBuffer> m_VSSceneBuffer;
		Ref<UniformBuffer> m_PSSceneBuffer;
	};

	class BasicGUIShader : public Shader
	{
	public:
		static Ref<BasicGUIShader> Create();

		void SetMVPBuffer(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view);
		void SetBasicShaderBuffer(const glm::vec3& color, float opacity, float texture_tiling = 1.0f);

	private:
		BasicGUIShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers);

	private:
		Ref<UniformBuffer> m_MVPBuffer;
		Ref<UniformBuffer> m_BasicShaderBuffer;
	};

	class BasicAnimShader : public Shader
	{
	public:
		static Ref<BasicAnimShader> Create();

		void SetMVPBuffer(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view);
		void SetBasicShaderBuffer(const glm::vec3& color, float opacity, float texture_tiling = 1.0f);
		void SetBoneTransformsBuffer(std::vector<glm::mat4>& bone_transforms);
		void SetSceneBuffers(const Light& light, const glm::vec3& camera_pos);

	private:
		BasicAnimShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers);

	private:
		Ref<UniformBuffer> m_MVPBuffer;
		Ref<UniformBuffer> m_BasicShaderBuffer;
		Ref<UniformBuffer> m_VSSceneBuffer;
		Ref<UniformBuffer> m_PSSceneBuffer;
		Ref<UniformBuffer> m_BoneTransformsBuffer;
	};

	class SkyboxShader : public Shader
	{
	public:
		static Ref<SkyboxShader> Create();

		void SetSkyboxBuffer(const glm::mat4& projection, const glm::mat4& view);

	private:
		SkyboxShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers);

	private:
		Ref<UniformBuffer> m_SkyboxBuffer;
	};

	class WaterShader : public Shader
	{
	public:
		static Ref<WaterShader> Create();

		void SetMVPBuffer(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view);
		void SetSceneBuffers(const Light& light, const glm::vec3& camera_pos, float tiling);
		void SetWaterBuffer(float wave_strength, float move_factor);

	private:
		WaterShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers);

	private:
		Ref<UniformBuffer> m_MVPBuffer;
		Ref<UniformBuffer> m_WaterBuffer;
		Ref<UniformBuffer> m_VSSceneBuffer;
		Ref<UniformBuffer> m_PSSceneBuffer;
	};

	class TerrainShader : public Shader
	{
	public:
		static Ref<TerrainShader> Create();

		void FillUniformBuffers(
			const glm::mat4& transform,
			const glm::mat4& projection, 
			const glm::mat4& view,
			const Light& light, 
			const glm::vec3& camera_pos,
			float UVTilingPrimary,
			float UVTilingSupport1,
			float UVTilingSupport2,
			float UVTilingSupport3,
			const glm::vec3& brush_center,
			float brush_radius
		);

	private:
		TerrainShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers);

	private:
		Ref<UniformBuffer> m_VSBuffer;
		Ref<UniformBuffer> m_DSBuffer;
		Ref<UniformBuffer> m_PSBuffer;
	};

	class GrassShader : public Shader
	{
	public:
		static Ref<GrassShader> Create();

		void SetMVPBuffer(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view);
		void SetSceneBuffers(const Light& light, const glm::vec3& camera_pos);
		void SetGrassBuffer(float WindRotation);

	private:
		GrassShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers);

	private:
		Ref<UniformBuffer> m_MVPBuffer;
		Ref<UniformBuffer> m_VSSceneBuffer;
		Ref<UniformBuffer> m_GrassBuffer;
	};
}
