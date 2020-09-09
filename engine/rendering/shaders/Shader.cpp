#include "Shader.h"

#include <rendering/Renderer.h>
#include <platform/rendering/shaders/DirectXShader.h>

#include "Shaders.h"

namespace ifr
{
	Ref<ShaderInstance> ShaderInstance::Create(const std::string& vs_src, const std::string& ps_src, const BufferLayout& layout)
	{
		return Ref<ShaderInstance>(new DirectXShaderInstance(vs_src, ps_src, layout));
	}

	Ref<ShaderInstance> ShaderInstance::Create(const std::string& vs_src, const std::string& hs_src, const std::string& ds_src, const std::string& ps_src, const BufferLayout& layout)
	{
		return Ref<ShaderInstance>(new DirectXShaderInstance(vs_src, hs_src, ds_src, ps_src, layout));
	}

	Ref<Shader> Shader::Create(const std::string& vs_src, const std::string& ps_src, const BufferLayout& layout, const std::initializer_list<Ref<UniformBuffer>>& buffers)
	{
		Ref<ShaderInstance> instance = ShaderInstance::Create(vs_src, ps_src, layout);
		return Ref<Shader>(new Shader(instance, buffers));
	}

	Ref<Shader> Shader::Create(const std::string& vs_src, const std::string& ps_src, const BufferLayout& layout, const std::vector<Ref<UniformBuffer>>& buffers)
	{
		Ref<ShaderInstance> instance = ShaderInstance::Create(vs_src, ps_src, layout);
		return Ref<Shader>(new Shader(instance, buffers));
	}

	Ref<Shader> Shader::Create(const std::string& vs_src, const std::string& hs_src, const std::string& ds_src, const std::string& ps_src, const BufferLayout& layout, const std::vector<Ref<UniformBuffer>>& buffers)
	{
		Ref<ShaderInstance> instance = ShaderInstance::Create(vs_src, hs_src, ds_src, ps_src, layout);
		return Ref<Shader>(new Shader(instance, buffers));
	}

	Ref<BasicShader> Shader::CreateDefaultBasicShader()
	{
		return BasicShader::Create();
	}

	Ref<BasicGUIShader> Shader::CreateDefaultBasicGUIShader()
	{
		return BasicGUIShader::Create();
	}

	Ref<BasicAnimShader> Shader::CreateDefaultBasicAnimShader()
	{
		return BasicAnimShader::Create();
	}

	Ref<TerrainShader> Shader::CreateDefaultTerrainShader()
	{
		return TerrainShader::Create();
	}

	Ref<SkyboxShader> Shader::CreateDefaultSkyboxShader()
	{
		return SkyboxShader::Create();
	}

	Ref<WaterShader> Shader::CreateDefaultWaterShader()
	{
		return WaterShader::Create();
	}

	Ref<GrassShader> Shader::CreateDefaultGrassShader()
	{
		return GrassShader::Create();
	}

	Shader::Shader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers) : m_ShaderInstance(instance)
	{
		for (auto buffer : buffers)
			m_BufferMap[buffer->GetName()] = buffer;
	}

	Shader::Shader(const Ref<ShaderInstance>& instance, const std::vector<Ref<UniformBuffer>>& buffers) : m_ShaderInstance(instance)
	{
		for (auto buffer : buffers)
			m_BufferMap[buffer->GetName()] = buffer;
	}

	Ref<UniformBuffer> Shader::GetBuffer(const std::string& name)
	{
		if (m_BufferMap.find(name) != m_BufferMap.end())
			return m_BufferMap.at(name);
		else
			return nullptr;
	}

	void Shader::AddBuffer(const std::string& name, Ref<UniformBuffer> buffer)
	{
		if (m_BufferMap.find(name) != m_BufferMap.end())
			m_BufferMap[name] = buffer;
	}

	void Shader::Bind()
	{
		m_ShaderInstance->Bind();
	}

	void Shader::Unbind()
	{
		m_ShaderInstance->Unbind();
	}
}
