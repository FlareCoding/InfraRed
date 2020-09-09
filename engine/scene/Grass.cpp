#include "Grass.h"
#include <model_data/shapes/Shapes.h>
#include <rendering/Renderer3D.h>
#include <random>

namespace ifr
{
	Ref<GrassPatch> GrassPatch::Create()
	{
		return Ref<GrassPatch>(new GrassPatch());
	}

	GrassPatch::GrassPatch()
	{
		// Vertex Data
		std::vector<GrassVertex> vertices;

		// First blade
		vertices.push_back(GrassVertex{ glm::vec3(-0.006f,	0,	-0.006f),		glm::vec2(0,	0) });
		vertices.push_back(GrassVertex{ glm::vec3(0,		0,	 0.006f),		glm::vec2(0.5f, 0) });
		vertices.push_back(GrassVertex{ glm::vec3(0.006f,	0,	-0.006f),		glm::vec2(1,	0) });

		vertices.push_back(GrassVertex{ glm::vec3(-0.006f,	0.5f,	-0.006f),	glm::vec2(0,	0.5f) });
		vertices.push_back(GrassVertex{ glm::vec3(0,		0.5f,	 0.006f),	glm::vec2(0.5f, 0.5f) });
		vertices.push_back(GrassVertex{ glm::vec3(0.006f,	0.5f,	-0.006f),	glm::vec2(1,	0.5f) });

		vertices.push_back(GrassVertex{ glm::vec3(0, 1.0f, 0), glm::vec2(0.5f, 1) });

		// Second blade
		vertices.push_back(GrassVertex{ glm::vec3(-0.01f,	0,	-0.01f),		glm::vec2(0,	0) });
		vertices.push_back(GrassVertex{ glm::vec3(0,		0,	 0.01f),		glm::vec2(0.5f, 0) });
		vertices.push_back(GrassVertex{ glm::vec3(0.01f,	0,	-0.01f),		glm::vec2(1,	0) });

		vertices.push_back(GrassVertex{ glm::vec3(-0.014f,	0.2f,	-0.014f),	glm::vec2(0,	0.5f) });
		vertices.push_back(GrassVertex{ glm::vec3(0,		0.2f,	 0.014f),	glm::vec2(0.5f, 0.5f) });
		vertices.push_back(GrassVertex{ glm::vec3(0.014f,	0.2f,	-0.014f),	glm::vec2(1,	0.5f) });

		vertices.push_back(GrassVertex{ glm::vec3(-0.02, 0.7f, -0.02), glm::vec2(0.5f, 1) });

		// Third blade
		vertices.push_back(GrassVertex{ glm::vec3(0.044f,	0,	 0.044f),		glm::vec2(0,	0) });
		vertices.push_back(GrassVertex{ glm::vec3(0.05f,	0,	 0.056f),		glm::vec2(0.5f, 0) });
		vertices.push_back(GrassVertex{ glm::vec3(0.046f,	0,	-0.044f),		glm::vec2(1,	0) });

		vertices.push_back(GrassVertex{ glm::vec3(0.05f,	0.4f,	 0.05f),		glm::vec2(0,	0.5f) });
		vertices.push_back(GrassVertex{ glm::vec3(0.056f,	0.4f,	 0.062f),		glm::vec2(0.5f, 0.5f) });
		vertices.push_back(GrassVertex{ glm::vec3(0.052f,	0.4f,	-0.038f),		glm::vec2(1,	0.5f) });

		vertices.push_back(GrassVertex{ glm::vec3(0.09f, 0.9f, 0.09f), glm::vec2(0.5f, 1) });

		// Instance Data
		for (uint32_t i = 0; i < m_InstanceCount; i++)
		{
			GrassInstance instance;
			instance.position = glm::vec3(((float)rand() / (float)(RAND_MAX)) * 8.5f - 8.5f, 0, ((float)rand() / (float)(RAND_MAX)) * 8.5f - 8.5f);

			std::random_device rd; // obtain a random number from hardware
			std::mt19937 eng(rd()); // seed the generator
			std::uniform_real_distribution<> distr_height(0.6, 1.0); // define the range
			std::uniform_real_distribution<> distr_rotation(-180, 180); // define the range

			instance.height = (float)distr_height(eng);
			instance.rotation = glm::radians((float)distr_rotation(eng));

			m_Instances.push_back(instance);
		}

		std::vector<int> indices = {
			// First blade
			0, 1, 4,		// Face 1 Tri 1 Level 1
			0, 4, 3,		// Face 1 Tri 2 Level 1

			2, 0, 3,		// Face 2 Tri 1 Level 1
			2, 3, 5,		// Face 2 Tri 2 Level 1

			1, 2, 5,		// Face 3 Tri 1 Level 1
			1, 5, 4,		// Face 3 Tri 2 Level 1

			4, 6, 3,
			3, 6, 5,
			5, 6, 4,

			// Second blade
			7, 8, 11,
			7, 11, 10,

			9, 7, 10,
			9, 10, 12,

			8, 9, 12,
			8, 12, 11,

			11, 13, 10,
			10, 13, 12,
			12, 13, 11,

			// Third blade
			14, 15, 18,
			14, 18, 17,

			16, 14, 17,
			16, 17, 19,

			15, 16, 19,
			15, 19, 18,

			18, 20, 17,
			17, 20, 19,
			19, 20, 18
		};

		// Creating the vertex array
		m_VertexArray = VertexArray::Create();

		// Per Vertex Buffer
		BufferLayout PerVertexLayout = {
			BufferElement("POSITION",	ShaderDataType::Float3, false, 0, BufferElementInputClass::INPUT_PER_VERTEX_DATA),
			BufferElement("UV",			ShaderDataType::Float2, false, 0, BufferElementInputClass::INPUT_PER_VERTEX_DATA)
		};

		Ref<VertexBuffer> PerVertexBuffer = VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(GrassVertex), sizeof(GrassVertex));
		PerVertexBuffer->SetLayout(PerVertexLayout);

		//// Per Instance Buffer
		BufferLayout PerInstanceLayout = {
			BufferElement("WORLD",		ShaderDataType::Float3, false, 1, BufferElementInputClass::INPUT_PER_INSTANCE_DATA),
			BufferElement("HEIGHT",		ShaderDataType::Float,  false, 1, BufferElementInputClass::INPUT_PER_INSTANCE_DATA),
			BufferElement("ROTATION",	ShaderDataType::Float,  false, 1, BufferElementInputClass::INPUT_PER_INSTANCE_DATA)
		};

		m_InstanceBuffer = VertexBuffer::Create(&m_Instances[0], m_Instances.size() * sizeof(GrassInstance), sizeof(GrassInstance));
		m_InstanceBuffer->SetLayout(PerInstanceLayout);

		// Index Buffer
		Ref<IndexBuffer> IndexBuffer = IndexBuffer::Create(indices);

		// Adding the buffers to the vertex array
		m_VertexArray->AddVertexBuffer(PerVertexBuffer);
		m_VertexArray->AddVertexBuffer(m_InstanceBuffer);
		m_VertexArray->SetIndexBuffer(IndexBuffer);

		// Creating the shader
		m_Shader = Shader::CreateDefaultGrassShader();
	}

	void GrassPatch::AddCluster(const glm::vec3& position)
	{
		
	}

	void GrassPatch::Render(ICamera& camera, const Light& light)
	{
		static float WindSpeed = 0.005f;

		if (m_WindDirection == 1)
		{
			m_WindRotation += (float)Time::GetTimestep() * WindSpeed;
			if (m_WindRotation > 5.0f)
				m_WindDirection = 2;
		}
		else
		{
			m_WindRotation -= (float)Time::GetTimestep() * WindSpeed;

			if (m_WindRotation < -1.0f)
				m_WindDirection = 1;
		}

		//===============================================//

		if (texture) texture->Bind(0);

		m_Shader->Bind();
		m_Shader->SetMVPBuffer(glm::translate(glm::scale(glm::mat4(1.0f), { Scale, Scale , Scale }), Position), camera.GetProjection(), camera.GetViewMatrix());
		m_Shader->SetSceneBuffers(light, camera.GetPosition());
		m_Shader->SetGrassBuffer(glm::radians(m_WindRotation));

		Renderer3D::DrawRawVertexArrayDataInstanced(m_VertexArray, m_InstanceCount);
	}
}
