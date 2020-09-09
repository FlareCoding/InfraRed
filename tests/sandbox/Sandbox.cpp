#include "Sandbox.h"

void SandboxApp::OnInitialize()
{
	 ImGuiRenderer::ShouldDisplayDebugInformation(true);

	 m_OrthoCamera = MakeRef<OrthographicCamera>();

	 m_FreeCamera = MakeRef<FreeRoamCamera>();
	 m_FreeCamera->SetPosition({ -5, 4, -5 });
	 m_FreeCamera->Speed = 80.0f;
	 m_FreeCamera->RotationSpeed = 100.0f;

	// Assets::RegisterAssetOnLoadCallback([this](const std::string& name) {
	// 	AssetOnLoad(name);
	// });

	CreateMaterials();

	IFR_LOG_SUCCESS("[<>] Sandbox Initialized [<>]");
}

void SandboxApp::OnUpdate(Timestep timestep)
{
	 if (Window::Get().IsCursorLocked())
	 {
	 	m_FreeCamera->Update();
	 }

	 Renderer::ClearScreenBuffers();
	 Renderer::ClearScreenColor(glm::vec4(0.373f, 0.460f, 0.527f, 1.0));

	 Renderer3D::BeginScene(*m_FreeCamera, m_Sun);
	 Renderer3D::DrawCube(m_Sun.position, 10, { 1, 1, 0 });
	 Renderer3D::EndScene();
}

void SandboxApp::OnImGuiDraw()
{
}

void SandboxApp::OnShutdown()
{
	IFR_LOG_SUCCESS("[<>] Sandbox Shutdown [<>]");
}

void SandboxApp::OnEvent(Event& e)
{
	if (Window::Get().IsCursorLocked())
	{
		if (m_FreeCamera) m_FreeCamera->OnEvent(e);
	}

	if (e.GetEventType() == EventType::MouseButtonPressed)
	{
		auto& evt = reinterpret_cast<MouseButtonPressedEvent&>(e);

		if (evt.GetMouseButton() == MouseButton::Right && !Window::Get().IsCursorLocked())
			Window::Get().SetCursorLocked(true);
	}

	if (e.GetEventType() == EventType::KeyReleased)
	{
		auto& evt = reinterpret_cast<KeyReleasedEvent&>(e);

		if (evt.GetKeyCode() == IFR_KEY_ESCAPE && Window::Get().IsCursorLocked())
		{
			Window::Get().SetCursorLocked(false);
		}
	}
}

void SandboxApp::AssetOnLoad(const std::string& name)
{
}

void SandboxApp::CreateMaterials()
{
	 CreateGlowMaterial();
	 CreateSimpleTextureMaterial();
	 CreateNormalMappedTextureMaterial();
}

void SandboxApp::CreateGlowMaterial()
{
	 ShaderGraph graph;
	 graph.SetLitMode(true);

	 auto color_alpha_node = MakeRef<SGCustomNode>(graph.CreateNodeID(), HLSL_FLOAT);
	 color_alpha_node->OutputName = "1.0f";

	 auto one_constant = MakeRef<SGCustomNode>(graph.CreateNodeID(), HLSL_FLOAT);
	 one_constant->OutputName = "1.5f";

	 auto color_div = MakeRef<SGCustomNode>(graph.CreateNodeID(), HLSL_FLOAT3);
	 color_div->OutputName = "2.0f";

	 auto uni_color_node = MakeRef<SGUniformValueNode>(graph.CreateNodeID(), HLSL_FLOAT3);
	 uni_color_node->OutputName = "u_UserColor";

	 auto uni_freq_node = MakeRef<SGUniformValueNode>(graph.CreateNodeID(), HLSL_FLOAT);
	 uni_freq_node->OutputName = "u_Frequency";

	 auto uni_intensity_node = MakeRef<SGUniformValueNode>(graph.CreateNodeID(), HLSL_FLOAT);
	 uni_intensity_node->OutputName = "u_Intensity";

	 auto sine_input_node = MakeRef<SGMultiplyNode>(graph.CreateNodeID(), HLSL_FLOAT);
	 sine_input_node->AddInput(graph.GetInputTimeNode());
	 sine_input_node->AddInput(uni_freq_node);
	 sine_input_node->OutputName = "SineInput";

	 auto sine_node = MakeRef<SGSineNode>(graph.CreateNodeID());
	 sine_node->AddInput(sine_input_node);
	 sine_node->OutputName = "SineOutput";

	 auto plus_one = MakeRef<SGAddNode>(graph.CreateNodeID(), HLSL_FLOAT);
	 plus_one->AddInput(sine_node);
	 plus_one->AddInput(one_constant);
	 plus_one->OutputName = "PlusOne";

	 auto over_two = MakeRef<SGDivideNode>(graph.CreateNodeID(), HLSL_FLOAT);
	 over_two->AddInput(plus_one);
	 over_two->AddInput(color_div);
	 over_two->OutputName = "OverTwo";

	 auto color_multiply_node = MakeRef<SGMultiplyNode>(graph.CreateNodeID(), HLSL_FLOAT3);
	 color_multiply_node->AddInput(uni_color_node);
	 color_multiply_node->AddInput(over_two);
	 color_multiply_node->OutputName = "ResultingColor";

	 auto color_node = MakeRef<SGFloat4Node>(graph.CreateNodeID());
	 color_node->OutputName = "Color";
	 color_node->AddInput(color_multiply_node);
	 color_node->AddInput(color_alpha_node);

	 graph.ConnectBaseColorInput(color_node);

	 auto m_GlowMaterial = Material::Create(graph);
}

void SandboxApp::CreateSimpleTextureMaterial()
{
	 ShaderGraph graph;
	 graph.SetLitMode(true);
	 graph.ShouldUseEnvironmentMap(true, 1);

	 auto texture_node = MakeRef<SGTextureSampler2DNode>(graph.CreateNodeID());
	 texture_node->AddInput(graph.GetInputUVNode());
	 texture_node->TextureSlot = 0;
	 texture_node->TextureName = "DiffuseTexture";
	 texture_node->OutputName = "TextureColor";

	 graph.ConnectBaseColorInput(texture_node);

	 auto m_SimpleTextureMaterial = Material::Create(graph);
}

void SandboxApp::CreateNormalMappedTextureMaterial()
{
	 ShaderGraph graph;
	 graph.SetLitMode(true);

	 auto diffuse_texture_node = MakeRef<SGTextureSampler2DNode>(graph.CreateNodeID());
	 diffuse_texture_node->AddInput(graph.GetInputUVNode());
	 diffuse_texture_node->TextureSlot = 0;
	 diffuse_texture_node->TextureName = "DiffuseTexture";
	 diffuse_texture_node->OutputName = "TextureColor";

	 graph.ShouldUseNormalMap(true);
	 graph.ConnectBaseColorInput(diffuse_texture_node);

	 auto m_NormalMappedTextureMaterial = Material::Create(graph);
}
