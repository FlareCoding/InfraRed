#include "ArenaLevel.h"

void ArenaLevel::OnCreate()
{
	CreateMaterials();

	m_HealthbarTextureFull = Assets::FindTexture("HealthbarFull");
	m_HealthbarTextureMid = Assets::FindTexture("HealthbarMid");
	m_HealthbarTextureLow = Assets::FindTexture("HealthbarLow");
	m_ExitGameTexture = Assets::FindTexture("ExitGameButton");
	m_TankModel = Assets::FindStaticModel("Tank");

	m_OrthoCamera = MakeRef<OrthographicCamera>();
	m_ThirdPersonCamera = MakeRef<ThirdPersonCamera>(nullptr);

	LevelScene = Scene::Create("Arena", nullptr, nullptr);
}

void ArenaLevel::OnRender()
{
	if (m_bShouldHideCursor && !Window::Get().IsCursorLocked())
	{
		Window::Get().SetCursorLocked(true);
	}

	if (!LevelScene->IsActive()) LevelScene->SetActive(true);

	if (m_ThirdPersonCamera && Window::Get().IsCursorLocked()) m_ThirdPersonCamera->Update();

	LevelScene->Update();
	RenderTankHealthbars();

	if (m_bPaused)
	{
		RenderPauseUI();
	}
}

void ArenaLevel::OnEvent(Event& e)
{
	if (m_ThirdPersonCamera && !m_bPaused) m_ThirdPersonCamera->OnEvent(e);

	if (e.GetEventType() == EventType::MouseButtonPressed)
	{
		auto& evt = reinterpret_cast<MouseButtonPressedEvent&>(e);

		if (!Window::Get().IsCursorLocked())
		{
			if (evt.GetMouseButton() == MouseButton::Right)
			{
				Window::Get().SetCursorLocked(true);
				m_bShouldHideCursor = true;
				m_bPaused = false;
			}
			else
			{
				ProcessUIMousePressEvent(evt);
			}
		}
	}

	if (e.GetEventType() == EventType::KeyReleased)
	{
		auto& evt = reinterpret_cast<KeyReleasedEvent&>(e);

		if (evt.GetKeyCode() == IFR_KEY_ESCAPE)
		{
			m_bShouldHideCursor = !m_bShouldHideCursor;
			m_bPaused = !m_bPaused;

			Window::Get().SetCursorLocked(!Window::Get().IsCursorLocked());
		}
	}
}

void ArenaLevel::OnBecameActive()
{
	m_bShouldHideCursor = true;
	m_bPaused = false;

	LevelScene->Erase();
	CreateScene();
}

void ArenaLevel::RenderPauseUI()
{
	Renderer2D::BeginScene(*m_OrthoCamera);
	Renderer2D::DrawQuad({ Window::Get().GetWidth() / 2, Window::Get().GetHeight() - 100 }, { 400, 160 }, m_ExitGameTexture);
	Renderer2D::EndScene();
}

void ArenaLevel::RenderTankHealthbars()
{
	Renderer2D::BeginScene(*m_OrthoCamera);

	LevelScene->ForEntityInScene([this](Entity entity) {
		auto Info = reinterpret_cast<InformationComponent*>(GetComponent(entity, ComponentType::Information));
		if (!Info) return;

		auto Health = Info->Param1;

		Ref<Texture> HealthbarTexture = nullptr;

		if (Health > 60)
			HealthbarTexture = m_HealthbarTextureFull;
		else if (Health > 20)
			HealthbarTexture = m_HealthbarTextureMid;
		else
			HealthbarTexture = m_HealthbarTextureLow;

		auto Transform = reinterpret_cast<TransformComponent*>(GetComponent(entity, ComponentType::Transform));

		auto WorldPos = Transform->Position;
		auto DistanceToTank = glm::distance(m_ThirdPersonCamera->GetPosition(), WorldPos);

		WorldPos.y += 15;

		float SizeScaleFactor = 1 - (DistanceToTank / m_MaxHealthbarVisibilityDistance);
		if (SizeScaleFactor < 0.16f) return;

		glm::vec3 ScreenPos = WorldToScreen(WorldPos, m_ThirdPersonCamera->GetViewMatrix(), m_ThirdPersonCamera->GetProjection(), { Window::Get().GetWidth(), Window::Get().GetHeight() });

		if (ScreenPos.z < 0.01f) return;

		Renderer2D::DrawQuad({ ScreenPos.x, ScreenPos.y }, glm::vec2(200, 120) * SizeScaleFactor, HealthbarTexture);
	});

	Renderer2D::EndScene();
}

Entity ArenaLevel::CreateTank()
{
	Entity TankEntity = CreateEntity();
	auto* Transform = reinterpret_cast<TransformComponent*>(GetComponent(TankEntity, ComponentType::Transform));
	Transform->Scale = { 0.004f, 0.004f, 0.004f };

	auto* RenderComponent = reinterpret_cast<StaticRenderComponent*>(AddComponent(TankEntity, ComponentType::StaticRender));
	RenderComponent->Model = m_TankModel;
	RenderComponent->Model.Material = MaterialInstance::Create(m_NormalMappedTextureMaterial);
	RenderComponent->Model.Material->AddTexture(Assets::FindTexture("TankDiffuse"));
	RenderComponent->Model.Material->AddTexture(Assets::FindTexture("TankNormal"));

	auto* Collider = reinterpret_cast<ColliderComponent*>(AddComponent(TankEntity, ComponentType::Collider));
	Collider->Weight = 200.0f;

	auto Info = reinterpret_cast<InformationComponent*>(AddComponent(TankEntity, ComponentType::Information));
	Info->Param1 = 100; // Health
	Info->Param2 = 1;	// Team

	return TankEntity;
}

void ArenaLevel::DestroyTank(Entity tank)
{
	LevelScene->RemoveAndDeleteEntity(tank);
}

void ArenaLevel::CreateScene()
{
	// Terrain
	auto heightmap = Texture2D::CreateFromColor({ 0, 0, 0 }, 2048, 2048, TextureFormat::RGBA16);
	heightmap->ShaderLocation = TextureShaderLocation::DOMAIN_SHADER;

	auto blendmap = Texture2D::CreateFromColor({ 0, 0, 0 }, 256, 256, TextureFormat::RGBA8);

	auto terrain = Terrain::Create(2048, 128);
	terrain->PrimaryTexture = Assets::FindTexture("TronFloor");
	terrain->PrimaryTextureTiling = 60.0f;
	terrain->Heightmap = heightmap;
	terrain->Blendmap = blendmap;

	LevelScene->SetTerrain(terrain);

	// Objects
	PlayerTank = CreateTank();
	auto Transform = reinterpret_cast<TransformComponent*>(GetComponent(PlayerTank, ComponentType::Transform));
	Transform->Position = { 100, 15, 100 };

	auto BulletMaterialInstance = MaterialInstance::Create(m_DiffuseTextureMaterial);
	BulletMaterialInstance->AddTexture(Texture2D::CreateFromColor(glm::vec3(40, 10, 0)));

	auto Script = MakeRef<PlayerControlScript>(PlayerTank, LevelScene, BulletMaterialInstance);

	auto ScriptComp = reinterpret_cast<ScriptComponent*>(AddComponent(PlayerTank, ComponentType::Script));
	ScriptComp->AddScript(Script);

	LevelScene->AddEntity(PlayerTank, { ComponentType::StaticRender, ComponentType::Collider, ComponentType::Script });

	OpponentTank = CreateTank();
	auto EnemyTransform = reinterpret_cast<TransformComponent*>(GetComponent(OpponentTank, ComponentType::Transform));
	EnemyTransform->Position = { 1020, 15, 1020 };

	LevelScene->AddEntity(OpponentTank, { ComponentType::StaticRender, ComponentType::Collider });

	// Camera
	m_ThirdPersonCamera->Target = Transform;
	m_ThirdPersonCamera->MinDistanceFromPlayer = 20.0f;
	m_ThirdPersonCamera->MaxDistanceFromPlayer = 60.0f;
	m_ThirdPersonCamera->DistanceFromPlayer = 40.0f;
	m_ThirdPersonCamera->IsTargetAffected = false;

	LevelScene->SetCamera(m_ThirdPersonCamera);
	m_bShouldHideCursor = true;
}

void ArenaLevel::CreateMaterials()
{
	CreateGlowMaterial();
	CreateSimpleTextureMaterial();
	CreateNormalMappedTextureMaterial();
}

void ArenaLevel::CreateGlowMaterial()
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

	m_GlowMaterial = Material::Create(graph);
}

void ArenaLevel::CreateSimpleTextureMaterial()
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

	m_DiffuseTextureMaterial = Material::Create(graph);
}

void ArenaLevel::CreateNormalMappedTextureMaterial()
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

	m_NormalMappedTextureMaterial = Material::Create(graph);
}

void ArenaLevel::ProcessUIMousePressEvent(MouseButtonPressedEvent& e)
{
	auto [x, y] = Input::GetMousePosition();

	if (e.GetMouseButton() == MouseButton::Left)
	{
		if (x > Window::Get().GetWidth() / 2 - 200 && x < Window::Get().GetWidth() / 2 + 200 && y > 0 && y < 160)
		{
			LevelManager::Get().SetActiveLevel("MainMenu");
		}
	}
}
