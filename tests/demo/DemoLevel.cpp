#include "DemoLevel.h"

void DemoLevel::OnCreate()
{
	m_OrthoCamera = MakeRef<OrthographicCamera>();
	m_FreeCamera = MakeRef<FreeRoamCamera>();
	m_FreeCamera->SetPosition({ -20, 26, 0 });
	m_FreeCamera->SetYaw(0);
	m_FreeCamera->RestrictMovement = true;

	LevelScene = Scene::Create("Arena", m_FreeCamera, nullptr);
	CreateScene();
}

void DemoLevel::OnRender()
{
	if (!LevelScene->IsActive()) LevelScene->SetActive(true);
	
	if (m_ShouldHideCursor && !Window::Get().IsCursorLocked())
		Window::Get().SetCursorLocked(true);

	if (Window::Get().IsCursorLocked()) 
		m_FreeCamera->Update();

	Renderer3D::BeginScene(*m_FreeCamera.get(), *LevelScene->GetLight());
	Renderer3D::DrawCube({ 170, 40, 50 }, 10, {1, 0.7f, 0});
	Renderer3D::EndScene();

	LevelScene->Update();
	m_FreeCamera->Translate({ 0.004f, 0, 0 });

	if (m_InitialCoverOpacity > 0.0f)
	{
		Renderer2D::BeginScene(*m_OrthoCamera.get());
		Renderer2D::DrawQuad({ Window::Get().GetWidth() / 2.0f, Window::Get().GetHeight() / 2.0f }, { Window::Get().GetWidth(), Window::Get().GetHeight() }, { 0, 0, 0 }, m_InitialCoverOpacity);
		Renderer2D::EndScene();
		m_InitialCoverOpacity -= 0.0006f;
	}
}

void DemoLevel::OnEvent(Event& e)
{
	if (Window::Get().IsCursorLocked()) 
		m_FreeCamera->OnEvent(e);

	if (e.GetEventType() == EventType::KeyReleased)
	{
		auto& evt = reinterpret_cast<KeyReleasedEvent&>(e);
		if (evt.GetKeyCode() == IFR_KEY_ESCAPE)
		{
			m_ShouldHideCursor = false;
			Window::Get().SetCursorLocked(!Window::Get().IsCursorLocked());
		}
	}

	if (e.GetEventType() == EventType::MouseButtonPressed)
	{
		auto& evt = reinterpret_cast<MouseButtonPressedEvent&>(e);
		if (evt.GetMouseButton() == MouseButton::Left && !Window::Get().IsCursorLocked())
		{
			m_ShouldHideCursor = true;
			Window::Get().SetCursorLocked(true);
		}
	}
}

void DemoLevel::OnBecameActive()
{
}

void DemoLevel::CreateScene()
{
	Ref<Skybox> skybox = Skybox::Create();
	skybox->texture = Assets::FindTexture("DemoSkybox");
	LevelScene->SetSkybox(skybox);

	Ref<WaterChunk> water_chunk = WaterChunk::Create();
	water_chunk->Size = 10000.0f;
	water_chunk->Tiling = 90.0f;
	water_chunk->WaveSpeed = 3.0f;
	water_chunk->WaveStrength = 10.0f;
	water_chunk->Position.y = 4;
	LevelScene->AddWaterChunk(water_chunk);

	ShaderGraph graph;
	graph.SetLitMode(true);

	auto texture_node = MakeRef<SGTextureSampler2DNode>(graph.CreateNodeID());
	texture_node->AddInput(graph.GetInputUVNode());
	texture_node->TextureSlot = 0;
	texture_node->TextureName = "DiffuseTexture";
	texture_node->OutputName = "TextureColor";

	graph.ConnectBaseColorInput(texture_node);
	auto material = Material::Create(graph);

	Boat = CreateEntity();
	auto* Transform = reinterpret_cast<TransformComponent*>(GetComponent(Boat, ComponentType::Transform));
	Transform->Scale = { 0.3f, 0.3f, 0.3f };

	auto* RenderComponent = reinterpret_cast<StaticRenderComponent*>(AddComponent(Boat, ComponentType::StaticRender));
	RenderComponent->Model = Assets::FindStaticModel("WoodenBoat");
	RenderComponent->Model.Material = MaterialInstance::Create(material);
	RenderComponent->Model.Material->AddTexture(Assets::FindTexture("WoodenBoatDiffuse"));

	auto* Collider = reinterpret_cast<ColliderComponent*>(AddComponent(Boat, ComponentType::Collider));
	Collider->Weight = 200.0f;
	Collider->Velocity.x = 0.004f;
	Collider->IsAffectedByGravity = false;

	LevelScene->AddEntity(Boat, { ComponentType::StaticRender, ComponentType::Collider });
}
