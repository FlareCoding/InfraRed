#include "Shaders.h"
#include "ShaderBuilder.h"
#include "UniformBufferSlotMappings.h"

namespace ifr
{
	static Ref<UniformBuffer> UniversalCreateMVPBuffer()
	{
		return UniformBuffer::Create(
			"MVPBuffer",
			{
				UniformVariable("Transform", sizeof(glm::mat4)),
				UniformVariable("Projection", sizeof(glm::mat4)),
				UniformVariable("View", sizeof(glm::mat4))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32_t)IFR_UB_SLOT::MVPMatrices
		);
	}

	static Ref<UniformBuffer> UniversalCreateBasicBuffer()
	{
		return UniformBuffer::Create(
			"BasicShaderBuffer",
			{
				UniformVariable("u_Color", sizeof(glm::vec3)),
				UniformVariable("u_Opacity", sizeof(float)),
				UniformVariable("u_Tiling", sizeof(float))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32_t)IFR_UB_SLOT::BasicShaderBuffer
		);
	}

	static Ref<UniformBuffer> UniversalCreateVSSceneBuffer()
	{
		return UniformBuffer::Create(
			"VSSceneBuffer",
			{
				UniformVariable("u_LightPosition", sizeof(glm::vec3)),
				UniformVariable("u_Padding00", sizeof(float)),
				UniformVariable("u_CameraPosition", sizeof(glm::vec3)),
				UniformVariable("u_Padding01", sizeof(float))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32_t)IFR_UB_SLOT::VSSceneBuffer
		);
	}

	static Ref<UniformBuffer> UniversalCreatePSSceneBuffer()
	{
		return UniformBuffer::Create(
			"PSSceneBuffer",
			{
				UniformVariable("u_LightColor", sizeof(glm::vec4)),
			},
			UniformBufferParentShader::PIXEL_SHADER,
			(uint32_t)IFR_UB_SLOT::PSSceneBuffer
		);
	}

	static void UniversalSetMVPBuffer(Ref<UniformBuffer>& buffer, const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view)
	{
		glm::mat4 mvp[3] = { transform, projection, view };
		buffer->SetBufferValue(mvp);
		buffer->UploadToShader();
	}

	static void UniversalSetBasicShaderBuffer(Ref<UniformBuffer>& buffer, const glm::vec3& color, float opacity, float texture_tiling)
	{
		float buf[5] = { color.x, color.y, color.z, opacity, texture_tiling };
		buffer->SetBufferValue(buf);
		buffer->UploadToShader();
	}

	static void UniversalSetVSSceneBuffer(Ref<UniformBuffer>& buffer, const glm::vec3& light_position, const glm::vec3& camera_pos)
	{
		float buf[8] = {
			light_position.x, light_position.y, light_position.z, 0,
			camera_pos.x, camera_pos.y, camera_pos.z, 0
		};

		buffer->SetBufferValue(buf);
		buffer->UploadToShader();
	}

	static void UniversalSetPSSceneBuffer(Ref<UniformBuffer>& buffer, const glm::vec4& light_color)
	{
		float buf[4] = { 
			light_color.r, light_color.g, light_color.b, light_color.a,
		};

		buffer->SetBufferValue(buf);
		buffer->UploadToShader();
	}

	static void UniversalSetSceneBuffers(Ref<UniformBuffer>& vs_scene_buffer, Ref<UniformBuffer>& ps_scene_buffer, const Light& light, const glm::vec3& camera_pos)
	{
		UniversalSetVSSceneBuffer(vs_scene_buffer, light.position, camera_pos);
		UniversalSetPSSceneBuffer(ps_scene_buffer, light.color);
	}

	Ref<BasicShader> BasicShader::Create()
	{
		auto mvp_buffer			 = UniversalCreateMVPBuffer();
		auto basic_shader_buffer = UniversalCreateBasicBuffer();
		auto vs_scene_buffer	 = UniversalCreateVSSceneBuffer();
		auto ps_scene_buffer	 = UniversalCreatePSSceneBuffer();

		BufferLayout layout = {
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 }
		};

		auto shader_src = ShaderBuilder::GetDefaultShaderCode();
		auto shader_instance = ShaderInstance::Create(shader_src.first, shader_src.second, layout);

		return Ref<BasicShader>(new BasicShader(shader_instance, { 
			mvp_buffer,
			basic_shader_buffer,
			vs_scene_buffer,
			ps_scene_buffer
		}));
	}

	BasicShader::BasicShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers)
		: Shader(instance, buffers)
	{
		m_MVPBuffer				= GetBuffer("MVPBuffer");
		m_BasicShaderBuffer		= GetBuffer("BasicShaderBuffer");
		m_VSSceneBuffer			= GetBuffer("VSSceneBuffer");
		m_PSSceneBuffer			= GetBuffer("PSSceneBuffer");
	}

	void BasicShader::SetMVPBuffer(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view)
	{
		UniversalSetMVPBuffer(m_MVPBuffer, transform, projection, view);
	}

	void BasicShader::SetBasicShaderBuffer(const glm::vec3& color, float opacity, float texture_tiling)
	{
		UniversalSetBasicShaderBuffer(m_BasicShaderBuffer, color, opacity, texture_tiling);
	}

	void BasicShader::SetSceneBuffers(const Light& light, const glm::vec3& camera_pos)
	{
		UniversalSetSceneBuffers(m_VSSceneBuffer, m_PSSceneBuffer, light, camera_pos);
	}

	Ref<BasicGUIShader> BasicGUIShader::Create()
	{
		auto mvp_buffer = UniversalCreateMVPBuffer();
		auto basic_shader_buffer = UniversalCreateBasicBuffer();

		BufferLayout layout = {
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 }
		};

		auto shader_src = ShaderBuilder::GetDefaultGUIShaderCode();
		auto shader_instance = ShaderInstance::Create(shader_src.first, shader_src.second, layout);

		return Ref<BasicGUIShader>(new BasicGUIShader(shader_instance, {
			mvp_buffer,
			basic_shader_buffer
		}));
	}

	BasicGUIShader::BasicGUIShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers) : Shader(instance, buffers)
	{
		m_MVPBuffer = GetBuffer("MVPBuffer");
		m_BasicShaderBuffer = GetBuffer("BasicShaderBuffer");
	}

	void BasicGUIShader::SetMVPBuffer(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view)
	{
		UniversalSetMVPBuffer(m_MVPBuffer, transform, projection, view);
	}

	void BasicGUIShader::SetBasicShaderBuffer(const glm::vec3& color, float opacity, float texture_tiling)
	{
		UniversalSetBasicShaderBuffer(m_BasicShaderBuffer, color, opacity, texture_tiling);
	}

	Ref<BasicAnimShader> BasicAnimShader::Create()
	{
		auto mvp_buffer			 = UniversalCreateMVPBuffer();
		auto basic_shader_buffer = UniversalCreateBasicBuffer();
		auto vs_scene_buffer	 = UniversalCreateVSSceneBuffer();
		auto ps_scene_buffer	 = UniversalCreatePSSceneBuffer();

		constexpr uint32_t MAX_BONES = 150;
		Ref<UniformBuffer> bone_transforms_buffer = UniformBuffer::Create(
			"BoneTransformsBuffer",
			{
				UniformVariable("u_BoneTransforms", sizeof(glm::mat4) * MAX_BONES)
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32_t)IFR_UB_SLOT::BoneTransforms
		);

		BufferLayout layout = {
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 },
			{ "TANGENT"	, ShaderDataType::Float3 },
			{ "BINORMAL", ShaderDataType::Float3 },
			{ "BONE_IDS", ShaderDataType::Int4 },
			{ "BONE_WEIGHTS", ShaderDataType::Float4 }
		};

		auto shader_src = ShaderBuilder::GetDefaultAnimShaderCode();
		auto shader_instance = ShaderInstance::Create(shader_src.first, shader_src.second, layout);

		return Ref<BasicAnimShader>(new BasicAnimShader(shader_instance, {
			mvp_buffer,
			basic_shader_buffer,
			vs_scene_buffer,
			ps_scene_buffer,
			bone_transforms_buffer
		}));
	}

	BasicAnimShader::BasicAnimShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers)
		: Shader(instance, buffers)
	{
		m_MVPBuffer				= GetBuffer("MVPBuffer");
		m_BasicShaderBuffer		= GetBuffer("BasicShaderBuffer");
		m_VSSceneBuffer			= GetBuffer("VSSceneBuffer");
		m_PSSceneBuffer			= GetBuffer("PSSceneBuffer");
		m_BoneTransformsBuffer	= GetBuffer("BoneTransformsBuffer");
	}

	void BasicAnimShader::SetMVPBuffer(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view)
	{
		UniversalSetMVPBuffer(m_MVPBuffer, transform, projection, view);
	}

	void BasicAnimShader::SetBasicShaderBuffer(const glm::vec3& color, float opacity, float texture_tiling)
	{
		UniversalSetBasicShaderBuffer(m_BasicShaderBuffer, color, opacity, texture_tiling);
	}

	void BasicAnimShader::SetBoneTransformsBuffer(std::vector<glm::mat4>& bone_transforms)
	{
		static glm::mat4 transforms[80];
		for (uint32_t i = 0; i < bone_transforms.size(); i++)
			transforms[i] = bone_transforms.at(i);
		m_BoneTransformsBuffer->SetBufferValue(&transforms[0]);
		m_BoneTransformsBuffer->UploadToShader();
	}

	void BasicAnimShader::SetSceneBuffers(const Light& light, const glm::vec3& camera_pos)
	{
		UniversalSetSceneBuffers(m_VSSceneBuffer, m_PSSceneBuffer, light, camera_pos);
	}

	Ref<SkyboxShader> SkyboxShader::Create()
	{
		auto skybox_buffer = UniformBuffer::Create(
			"SkyboxBuffer",
			{
				UniformVariable("Projection", sizeof(glm::mat4)),
				UniformVariable("View", sizeof(glm::mat4))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32_t)IFR_UB_SLOT::SkyboxBuffer
		);

		BufferLayout layout = {
			{ "POSITION", ShaderDataType::Float3 },
		};

		auto shader_src = ShaderBuilder::GetDefaultSkyboxShaderCode();
		auto shader_instance = ShaderInstance::Create(shader_src.first, shader_src.second, layout);

		return Ref<SkyboxShader>(new SkyboxShader(shader_instance, {
			skybox_buffer
		}));
	}

	SkyboxShader::SkyboxShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers)
		: Shader(instance, buffers)
	{
		m_SkyboxBuffer = GetBuffer("SkyboxBuffer");
	}
	
	void SkyboxShader::SetSkyboxBuffer(const glm::mat4& projection, const glm::mat4& view)
	{
		glm::mat4 mvp[2] = { projection, view };

		// removing translation from the view matrix
		(mvp[1])[3][0] = 0;
		(mvp[1])[3][1] = 0;
		(mvp[1])[3][2] = 0;

		m_SkyboxBuffer->SetBufferValue(mvp);
		m_SkyboxBuffer->UploadToShader();
	}	
	
	Ref<WaterShader> WaterShader::Create()
	{
		auto mvp_buffer = UniversalCreateMVPBuffer();
		auto ps_scene_buffer = UniversalCreatePSSceneBuffer();

		auto vs_scene_buffer = UniformBuffer::Create(
			"VSSceneBuffer",
			{
				UniformVariable("u_LightPosition", sizeof(glm::vec3)),
				UniformVariable("u_Padding00", sizeof(float)),
				UniformVariable("u_CameraPosition", sizeof(glm::vec3)),
				UniformVariable("u_Padding01", sizeof(float)),
				UniformVariable("u_Tiling", sizeof(float))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32_t)IFR_UB_SLOT::VSSceneBuffer
		);;

		Ref<UniformBuffer> water_buffer = UniformBuffer::Create(
			"WaterBuffer",
			{
				UniformVariable("u_WaveStrength", sizeof(float)),
				UniformVariable("u_MoveFactor", sizeof(float))
			},
			UniformBufferParentShader::PIXEL_SHADER,
			(uint32_t)IFR_UB_SLOT::WaterBuffer
		);

		BufferLayout layout = {
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 }
		};

		auto shader_src = ShaderBuilder::GetDefaultWaterShaderCode();
		auto shader_instance = ShaderInstance::Create(shader_src.first, shader_src.second, layout);

		return Ref<WaterShader>(new WaterShader(shader_instance, {
			mvp_buffer,
			water_buffer,
			vs_scene_buffer,
			ps_scene_buffer
		}));
	}

	WaterShader::WaterShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers) : Shader(instance, buffers)
	{
		m_MVPBuffer = GetBuffer("MVPBuffer");
		m_WaterBuffer = GetBuffer("WaterBuffer");
		m_VSSceneBuffer = GetBuffer("VSSceneBuffer");
		m_PSSceneBuffer = GetBuffer("PSSceneBuffer");
	}

	void WaterShader::SetMVPBuffer(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view)
	{
		UniversalSetMVPBuffer(m_MVPBuffer, transform, projection, view);
	}
	
	void WaterShader::SetSceneBuffers(const Light& light, const glm::vec3& camera_pos, float tiling)
	{
		float vsbuf[9] = {
			light.position.x, light.position.y, light.position.z, 0,
			camera_pos.x, camera_pos.y, camera_pos.z, 0,
			tiling
		};

		m_VSSceneBuffer->SetBufferValue(vsbuf);
		m_VSSceneBuffer->UploadToShader();

		UniversalSetPSSceneBuffer(m_PSSceneBuffer, light.color);
	}
	
	void WaterShader::SetWaterBuffer(float wave_strength, float move_factor)
	{
		float buf[2] = { wave_strength, move_factor };
		m_WaterBuffer->SetBufferValue(buf);
		m_WaterBuffer->UploadToShader();
	}
	
	Ref<TerrainShader> TerrainShader::Create()
	{
		auto vs_buffer = UniformBuffer::Create(
			"VSBuffer",
			{
				UniformVariable("WorldTransform", sizeof(glm::mat4)),
				UniformVariable("u_CameraPosition", sizeof(glm::vec3))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			0
		);

		auto ds_buffer = UniformBuffer::Create(
			"DSBuffer",
			{
				UniformVariable("Transform", sizeof(glm::mat4)),
				UniformVariable("Projection", sizeof(glm::mat4)),
				UniformVariable("View", sizeof(glm::mat4)),
				UniformVariable("u_CameraPosition", sizeof(glm::vec3)),
				UniformVariable("u_Padding00", sizeof(float)),
				UniformVariable("u_LightPosition", sizeof(glm::vec3)),
				UniformVariable("u_Padding01", sizeof(float)),
				UniformVariable("u_BrushCenter", sizeof(glm::vec3)),
				UniformVariable("u_BrushRadius", sizeof(float)),
			},
			UniformBufferParentShader::DOMAIN_SHADER,
			1
		);

		auto ps_buffer = UniformBuffer::Create(
			"PSBuffer",
			{
				UniformVariable("u_UVTilingPrimary", sizeof(float)),
				UniformVariable("u_UVTilingSupport1", sizeof(float)),
				UniformVariable("u_UVTilingSupport2", sizeof(float)),
				UniformVariable("u_UVTilingSupport3", sizeof(float)),
				UniformVariable("u_LightColor", sizeof(glm::vec3))
			},
			UniformBufferParentShader::PIXEL_SHADER,
			2
		);

		BufferLayout layout = {
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 }
		};

		auto shader_src = ShaderBuilder::GetTerrainShaderCode();
		auto shader_instance = ShaderInstance::Create(shader_src[0], shader_src[1], shader_src[2], shader_src[3], layout);

		return Ref<TerrainShader>(new TerrainShader(shader_instance, {
			vs_buffer,
			ds_buffer,
			ps_buffer
		}));
	}

	TerrainShader::TerrainShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers)
		: Shader(instance, buffers)
	{
		m_VSBuffer = GetBuffer("VSBuffer");
		m_DSBuffer = GetBuffer("DSBuffer");
		m_PSBuffer = GetBuffer("PSBuffer");
	}

	void TerrainShader::FillUniformBuffers(
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
	)
	{
		m_VSBuffer->SetVariableValue("WorldTransform", (void*)&transform);
		m_VSBuffer->SetVariableValue("u_CameraPosition", (void*)&camera_pos);
		m_VSBuffer->UploadToShader();

		m_DSBuffer->SetVariableValue("Transform", (void*)&transform);
		m_DSBuffer->SetVariableValue("Projection", (void*)&projection);
		m_DSBuffer->SetVariableValue("View", (void*)&view);
		m_DSBuffer->SetVariableValue("u_CameraPosition", (void*)&camera_pos);
		m_DSBuffer->SetVariableValue("u_LightPosition", (void*)&light.position);
		m_DSBuffer->SetVariableValue("u_BrushCenter", (void*)&brush_center);
		m_DSBuffer->SetVariableValue("u_BrushRadius", (void*)&brush_radius);
		m_DSBuffer->UploadToShader();

		float ps_buffer[8] = { UVTilingPrimary, UVTilingSupport1, UVTilingSupport2, UVTilingSupport3, light.color.x, light.color.y, light.color.z, light.color.w };

		m_PSBuffer->SetBufferValue(ps_buffer);
		m_PSBuffer->UploadToShader();
	}
	
	Ref<GrassShader> GrassShader::Create()
	{
		auto mvp_buffer = UniversalCreateMVPBuffer();
		auto vs_scene_buffer = UniversalCreateVSSceneBuffer();
		
		auto grass_buffer = UniformBuffer::Create(
			"GrassBuffer",
			{
				UniformVariable("u_WindRotation", sizeof(float))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			1
		);

		BufferLayout layout = {
			BufferElement("POSITION",	ShaderDataType::Float3, false, 0, BufferElementInputClass::INPUT_PER_VERTEX_DATA),
			BufferElement("UV",			ShaderDataType::Float2, false, 0, BufferElementInputClass::INPUT_PER_VERTEX_DATA),
			BufferElement("WORLD",		ShaderDataType::Float3, false, 1, BufferElementInputClass::INPUT_PER_INSTANCE_DATA),
			BufferElement("HEIGHT",		ShaderDataType::Float,  false, 1, BufferElementInputClass::INPUT_PER_INSTANCE_DATA),
			BufferElement("ROTATION",	ShaderDataType::Float,  false, 1, BufferElementInputClass::INPUT_PER_INSTANCE_DATA)
		};

		auto shader_src = ShaderBuilder::GetDefaultGrassShaderCode();
		auto shader_instance = ShaderInstance::Create(shader_src.first, shader_src.second, layout);

		return Ref<GrassShader>(new GrassShader(shader_instance, {
			mvp_buffer,
			vs_scene_buffer,
			grass_buffer
		}));
	}
	
	GrassShader::GrassShader(const Ref<ShaderInstance>& instance, const std::initializer_list<Ref<UniformBuffer>>& buffers) : Shader(instance, buffers)
	{
		m_MVPBuffer		= GetBuffer("MVPBuffer");
		m_VSSceneBuffer = GetBuffer("VSSceneBuffer");
		m_GrassBuffer	= GetBuffer("GrassBuffer");
	}

	void GrassShader::SetMVPBuffer(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view)
	{
		UniversalSetMVPBuffer(m_MVPBuffer, transform, projection, view);
	}
	
	void GrassShader::SetSceneBuffers(const Light& light, const glm::vec3& camera_pos)
	{	
		UniversalSetVSSceneBuffer(m_VSSceneBuffer, light.position, camera_pos);
	}
	
	void GrassShader::SetGrassBuffer(float WindRotation)
	{
		m_GrassBuffer->SetBufferValue(&WindRotation);
		m_GrassBuffer->UploadToShader();
	}
}
