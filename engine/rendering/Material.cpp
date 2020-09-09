#include "Material.h"

namespace ifr
{
	Ref<Material> Material::Create(ShaderGraph& graph)
	{
		return Ref<Material>(new Material(graph));
	}

	Material::Material(ShaderGraph& graph) : m_ShaderGraph(graph)
	{
		m_ShaderGraph.GenerateHLSLCode();
	}

	Ref<MaterialInstance> MaterialInstance::Create(const Ref<Material>& material, ShaderGraphMeshType mesh_type)
	{
		return Ref<MaterialInstance>(new MaterialInstance(material, mesh_type));
	}

	MaterialInstance::MaterialInstance(const Ref<Material>& material, ShaderGraphMeshType mesh_type) : m_MaterialRef(material), m_MeshType(mesh_type)
	{
		CreateShader();

		// Default material properties
		SetRoughness(1.0f);
	}

	void MaterialInstance::CreateShader()
	{
		auto vs_source = m_MaterialRef->m_ShaderGraph.GetVertexShaderHLSLSource(m_MeshType);
		auto ps_source = m_MaterialRef->m_ShaderGraph.GetPixelShaderHLSLSource();
		auto uniforms  = m_MaterialRef->m_ShaderGraph.GetMaterialUniforms();
		
		BufferLayout layout;
		if (m_MeshType == MESH_STATIC)
		{
			layout = {
				{ "POSITION", ShaderDataType::Float3 },
				{ "UV"		, ShaderDataType::Float2 },
				{ "NORMAL"	, ShaderDataType::Float3 },
				{ "TANGENT"	, ShaderDataType::Float3 },
				{ "BINORMAL", ShaderDataType::Float3 }
			};
		}
		else
		{
			layout = {
				{ "POSITION", ShaderDataType::Float3 },
				{ "UV"		, ShaderDataType::Float2 },
				{ "NORMAL"	, ShaderDataType::Float3 },
				{ "TANGENT"	, ShaderDataType::Float3 },
				{ "BINORMAL", ShaderDataType::Float3 },
				{ "BONE_IDS", ShaderDataType::Int4 },
				{ "BONE_WEIGHTS", ShaderDataType::Float4 }
			};
		}

		auto model_view_projection_buffer = UniformBuffer::Create(
			"ModelViewProjectionBuffer",
			{
				UniformVariable("Transform", sizeof(glm::mat4)),
				UniformVariable("Projection", sizeof(glm::mat4)),
				UniformVariable("View", sizeof(glm::mat4))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			0
		);

		auto vsbuffer = UniformBuffer::Create(
			"VSBuffer",
			{
				UniformVariable("u_LightPosition", sizeof(glm::vec3)),
				UniformVariable("u_Padding00", sizeof(float)),
				UniformVariable("u_CameraPosition", sizeof(glm::vec3)),
				UniformVariable("u_Padding01", sizeof(float)),
				UniformVariable("u_ClipPlane", sizeof(glm::vec4))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			1
		);

		auto psbuffer = UniformBuffer::Create(
			"PSBuffer",
			{
				UniformVariable("u_LightColor", sizeof(glm::vec4)),
				UniformVariable("u_Time", sizeof(float))
			},
			UniformBufferParentShader::PIXEL_SHADER,
			2
		);

		auto material_buffer = UniformBuffer::Create(
			"MaterialBuffer",
			uniforms,
			UniformBufferParentShader::PIXEL_SHADER,
			3
		);

		std::vector<Ref<UniformBuffer>> uniform_buffers = {
			model_view_projection_buffer,
			vsbuffer,
			psbuffer,
			material_buffer 
		};

		// if mesh is animated, add the bone transforms buffer
		if (m_MeshType == MESH_ANIMATED)
		{
			auto bone_transforms_buffer = UniformBuffer::Create(
				"BoneTransformsBuffer",
				{
					UniformVariable("u_BoneTransforms", sizeof(glm::mat4) * 80)
				},
				UniformBufferParentShader::VERTEX_SHADER,
				6
			);

			uniform_buffers.push_back(bone_transforms_buffer);
		}

		m_Shader = Shader::Create(vs_source, ps_source, layout, uniform_buffers);

		m_MVPBuffer		 = m_Shader->GetBuffer("ModelViewProjectionBuffer");
		m_VSBuffer		 = m_Shader->GetBuffer("VSBuffer");
		m_PSBuffer		 = m_Shader->GetBuffer("PSBuffer");
		m_MaterialBuffer = m_Shader->GetBuffer("MaterialBuffer");

		if (m_MeshType == MESH_ANIMATED)
			m_BoneTransformsBuffer = m_Shader->GetBuffer("BoneTransformsBuffer");
	}

	void MaterialInstance::SetRoughness(float value)
	{
		m_Roughness = value;
		SetUniform<float>("u_Roughness", m_Roughness);
	}

	void MaterialInstance::Bind()
	{
		m_Shader->Bind();
	}

	void MaterialInstance::Unbind()
	{
		m_Shader->Unbind();
	}

	void MaterialInstance::AddTexture(const Ref<Texture>& texture)
	{
		m_Textures.push_back(texture);
	}

	Ref<Texture> MaterialInstance::GetTexture(size_t idx)
	{
		return m_Textures.at(idx);
	}

	void MaterialInstance::SetObjectSceneProperties(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& camera_pos, Light& light, const glm::vec4& clip_plane)
	{
		glm::mat4 mvp[3] = { transform, projection, view };
		m_MVPBuffer->SetBufferValue(mvp);
		m_MVPBuffer->UploadToShader();

		float vsbuf[12] = {
			light.position.x, light.position.y, light.position.z, 0,
			camera_pos.x, camera_pos.y, camera_pos.z, 0,
			clip_plane.x, clip_plane.y, clip_plane.z, clip_plane.w
		};

		m_VSBuffer->SetBufferValue(vsbuf);
		m_VSBuffer->UploadToShader();

		float psbuf[5] = {
			light.color.r, light.color.g, light.color.b, light.color.a,
			(float)Time::GetElapsedTime()
		};

		m_PSBuffer->SetBufferValue(psbuf);
		m_PSBuffer->UploadToShader();

		m_MaterialBuffer->UploadToShader();
	}
	
	void MaterialInstance::SetObjectSceneProperties(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& camera_pos, Light& light, std::vector<glm::mat4>& bone_transforms, const glm::vec4& clip_plane)
	{
		glm::mat4 mvp[3] = { transform, projection, view };
		m_MVPBuffer->SetBufferValue(mvp);
		m_MVPBuffer->UploadToShader();

		float vsbuf[12] = {
			light.position.x, light.position.y, light.position.z, 0,
			camera_pos.x, camera_pos.y, camera_pos.z, 0,
			clip_plane.x, clip_plane.y, clip_plane.z, clip_plane.w
		};

		m_VSBuffer->SetBufferValue(vsbuf);
		m_VSBuffer->UploadToShader();

		float psbuf[5] = {
			light.color.r, light.color.g, light.color.b, light.color.a,
			(float)Time::GetElapsedTime()
		};

		m_PSBuffer->SetBufferValue(psbuf);
		m_PSBuffer->UploadToShader();

		m_MaterialBuffer->UploadToShader();

		static glm::mat4 transforms[80];
		for (uint32_t i = 0; i < bone_transforms.size(); i++)
			transforms[i] = bone_transforms.at(i);
		m_BoneTransformsBuffer->SetBufferValue(&transforms[0]);
		m_BoneTransformsBuffer->UploadToShader();
	}
}
