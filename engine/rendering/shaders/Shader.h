#pragma once
#include <core/IFRCore.h>
#include "UniformBuffer.h"
#include "Rendering/BufferLayout.h"
#include <math/IFRMath.h>

namespace ifr
{
	class ShaderInstance
	{
	public:
		virtual ~ShaderInstance() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Ref<ShaderInstance> Create(const std::string& vs_src, const std::string& ps_src, const BufferLayout& layout);
		static Ref<ShaderInstance> Create(const std::string& vs_src, const std::string& hs_src, const std::string& ds_src, const std::string& ps_src, const BufferLayout& layout);
	};

	class BasicShader;
	class BasicGUIShader;
	class BasicAnimShader;
	class TerrainShader;
	class SkyboxShader;
	class WaterShader;
	class GrassShader;

	class Shader
	{
	public:
		void Bind();
		void Unbind();

		static Ref<Shader> Create(const std::string& vs_src, const std::string& ps_src, const BufferLayout& layout, const std::initializer_list<Ref<UniformBuffer>>& buffers = {});
		static Ref<Shader> Create(const std::string& vs_src, const std::string& ps_src, const BufferLayout& layout, const std::vector<Ref<UniformBuffer>>& buffers);
		static Ref<Shader> Create(const std::string& vs_src, const std::string& hs_src, const std::string& ds_src, const std::string& ps_src, const BufferLayout& layout, const std::vector<Ref<UniformBuffer>>& buffers);

		static Ref<BasicShader>		CreateDefaultBasicShader();
		static Ref<BasicGUIShader>	CreateDefaultBasicGUIShader();
		static Ref<BasicAnimShader>	CreateDefaultBasicAnimShader();
		static Ref<TerrainShader>		CreateDefaultTerrainShader();
		static Ref<SkyboxShader>		CreateDefaultSkyboxShader();
		static Ref<WaterShader>		CreateDefaultWaterShader();
		static Ref<GrassShader>		CreateDefaultGrassShader();

		Ref<UniformBuffer> GetBuffer(const std::string& name);
		void AddBuffer(const std::string& name, Ref<UniformBuffer> buffer);

	protected:
		Shader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers);
		Shader(const Ref<ShaderInstance>& instance, const std::vector<Ref<UniformBuffer>>& buffers);

		Ref<ShaderInstance> m_ShaderInstance = nullptr;
		std::map<std::string, Ref<UniformBuffer>> m_BufferMap;
	};
}
