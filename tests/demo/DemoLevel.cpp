#include "DemoLevel.h"
#include <random>
#include <thread>

void DemoLevel::OnCreate()
{
	m_OrthoCamera = MakeRef<OrthographicCamera>();
	m_FreeCamera = MakeRef<FreeRoamCamera>();
	m_FreeCamera->SetPosition({ -16, 28, 0 });
	m_FreeCamera->SetYaw(0);
	m_FreeCamera->RotationSpeed = 50.0f;
	m_FreeCamera->RestrictMovement = true;

	LevelScene = Scene::Create("Arena", m_FreeCamera, nullptr);
	CreateScene();
}

void DemoLevel::OnRender()
{
	double DemoRunningTime = Time::GetElapsedTime() - m_DemoStartTime;

	if (!LevelScene->IsActive()) LevelScene->SetActive(true);
	
	if (m_ShouldHideCursor && !Window::Get().IsCursorLocked())
		Window::Get().SetCursorLocked(true);

	if (Window::Get().IsCursorLocked()) 
		m_FreeCamera->Update();

	if (DemoRunningTime < 47)
	{
		auto* BoatTransform = reinterpret_cast<TransformComponent*>(GetComponent(m_BoatEntity, ComponentType::Transform));
		BoatTransform->Rotation.z = sinf((float)Time::GetElapsedTime()) * 0.03f;

		auto* SailorTransform = reinterpret_cast<TransformComponent*>(GetComponent(m_SailorEntity, ComponentType::Transform));
		SailorTransform->Rotation.z = sinf((float)Time::GetElapsedTime()) * 0.03f;
	}

	if (DemoRunningTime > 8)
	{
		auto* SailorRenderComponent = reinterpret_cast<AnimatedRenderComponent*>(GetComponent(m_SailorEntity, ComponentType::AnimatedRender));

		if (!m_StartedSailorAnimation)
		{
			SailorRenderComponent->Model.GetActiveAnimation().AnimationSpeed = 1.0f;
			m_StartedSailorAnimation = true;
			AudioEngine::Get().GetAudioSource("Greeting")->Start();
		}

		if (!SailorRenderComponent->Model.GetActiveAnimation().IsPlaying())
		{
			SailorRenderComponent->Model.SetActiveAnimation("Armature|TalkingAnimation");
			SailorRenderComponent->Model.GetActiveAnimation().Play();
			AudioEngine::Get().GetAudioSource("Introduction")->Start();
		}
	}

	if (DemoRunningTime > 39 && !m_StartedSharkAnimation)
	{
		auto* SailorRenderComponent = reinterpret_cast<AnimatedRenderComponent*>(GetComponent(m_SailorEntity, ComponentType::AnimatedRender));
		SailorRenderComponent->Model.SetActiveAnimation("Armature|LookAShark");
		SailorRenderComponent->Model.GetActiveAnimation().Play();
		AudioEngine::Get().GetAudioSource("LookAShark")->Start();
		m_StartedSharkAnimation = true;
	}

	if (DemoRunningTime > 39 && DemoRunningTime < 43)
	{
		auto* SharkTransform = reinterpret_cast<TransformComponent*>(GetComponent(m_SharkEntity, ComponentType::Transform));
		SharkTransform->Position -= glm::vec3(0.14f * (float)Time::GetTimestep(), 0, 0.14f * (float)Time::GetTimestep());
	}

	if (DemoRunningTime > 43 && !m_StartedSharkJumpingAnimation)
	{
		auto* SailorRenderComponent = reinterpret_cast<AnimatedRenderComponent*>(GetComponent(m_SailorEntity, ComponentType::AnimatedRender));
		SailorRenderComponent->Model.GetActiveAnimation().AnimationSpeed = 0;

		auto* SharkRenderComponent = reinterpret_cast<AnimatedRenderComponent*>(GetComponent(m_SharkEntity, ComponentType::AnimatedRender));
		SharkRenderComponent->Model.GetActiveAnimation().Stop();
		SharkRenderComponent->Model.SetActiveAnimation("Armature|Swimming");
		SharkRenderComponent->Model.GetActiveAnimation().Play();
		AudioEngine::Get().GetAudioSource("SlowMotion")->Start();

		m_StartedSharkJumpingAnimation = true;
	}

	if (DemoRunningTime > 46.4)
	{
		auto* SharkRenderComponent = reinterpret_cast<AnimatedRenderComponent*>(GetComponent(m_SharkEntity, ComponentType::AnimatedRender));
		SharkRenderComponent->Model.GetActiveAnimation().Stop();
		SharkRenderComponent->Enabled = false;
	}
	else if (DemoRunningTime > 45)
	{
		auto* SharkRenderComponent = reinterpret_cast<AnimatedRenderComponent*>(GetComponent(m_SharkEntity, ComponentType::AnimatedRender));
		SharkRenderComponent->Model.GetActiveAnimation().AnimationSpeed = 1;
	}
	else if (DemoRunningTime > 43.4)
	{
		auto* SharkRenderComponent = reinterpret_cast<AnimatedRenderComponent*>(GetComponent(m_SharkEntity, ComponentType::AnimatedRender));
		SharkRenderComponent->Model.GetActiveAnimation().AnimationSpeed = 0.2f;
	}

	if (DemoRunningTime > 48)
	{
		if (m_InitialCoverOpacity < 100.0f)
		{
			Renderer2D::BeginScene(*m_OrthoCamera.get());
			Renderer2D::DrawQuad({ Window::Get().GetWidth() / 2.0f, Window::Get().GetHeight() / 2.0f }, { Window::Get().GetWidth(), Window::Get().GetHeight() }, { 0, 0, 0 }, m_InitialCoverOpacity);
			Renderer2D::EndScene();
			m_InitialCoverOpacity += 0.0008f;
		}
	}

	LevelScene->Update();
	m_FreeCamera->Translate({ 0.004f, 0, 0 });

	if (DemoRunningTime < 10 && m_InitialCoverOpacity > 0.0f)
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
	m_DemoStartTime = Time::GetElapsedTime();

	AudioEngine::Get().GetAudioSource("OceanWaves")->SetVolume(30);
	AudioEngine::Get().GetAudioSource("OceanWaves")->Start();

	auto* SailorRenderComponent = reinterpret_cast<AnimatedRenderComponent*>(GetComponent(m_SailorEntity, ComponentType::AnimatedRender));
	SailorRenderComponent->Model.GetActiveAnimation().Play();
	SailorRenderComponent->Model.GetActiveAnimation().AnimationSpeed = 0.0f;
}

void DemoLevel::CreateScene()
{
	Ref<Skybox> skybox = Skybox::Create();
	skybox->texture = Assets::FindTexture("DemoSkybox");
	LevelScene->SetSkybox(skybox);

	Ref<WaterChunk> water_chunk = WaterChunk::Create();
	water_chunk->Size = 10000.0f;
	water_chunk->Tiling = 90.0f;
	water_chunk->WaveSpeed = 5.0f;
	water_chunk->WaveStrength = 100.0f;
	water_chunk->Position.y = 4;
	LevelScene->AddWaterChunk(water_chunk);

	CreateMaterials();

	std::thread t([this]() {
		CreateVegetationEntities();
	});
	t.detach();

	CreateBoatEntity();
	CreateSailorEntity();
	CreateSharkEntity();
}

void DemoLevel::CreateMaterials()
{
	ShaderGraph graph;
	graph.SetLitMode(true);

	auto texture_node = MakeRef<SGTextureSampler2DNode>(graph.CreateNodeID());
	texture_node->AddInput(graph.GetInputUVNode());
	texture_node->TextureSlot = 0;
	texture_node->TextureName = "DiffuseTexture";
	texture_node->OutputName = "TextureColor";

	graph.ConnectBaseColorInput(texture_node);
	m_DefaultMaterial = Material::Create(graph);

	ShaderGraph normal_mapped_graph;
	normal_mapped_graph.SetLitMode(true);
	normal_mapped_graph.ShouldUseNormalMap(true);

	auto normal_mapped_texture_node = MakeRef<SGTextureSampler2DNode>(normal_mapped_graph.CreateNodeID());
	normal_mapped_texture_node->AddInput(normal_mapped_graph.GetInputUVNode());
	normal_mapped_texture_node->TextureSlot = 0;
	normal_mapped_texture_node->TextureName = "DiffuseTexture";
	normal_mapped_texture_node->OutputName = "TextureColor";

	normal_mapped_graph.ConnectBaseColorInput(normal_mapped_texture_node);
	m_NormalMappedMaterial = Material::Create(normal_mapped_graph);
}

void DemoLevel::CreateBoatEntity()
{
	m_BoatEntity = CreateEntity();
	auto* Transform = reinterpret_cast<TransformComponent*>(GetComponent(m_BoatEntity, ComponentType::Transform));
	Transform->Scale = { 0.3f, 0.3f, 0.3f };

	auto* RenderComponent = reinterpret_cast<StaticRenderComponent*>(AddComponent(m_BoatEntity, ComponentType::StaticRender));
	RenderComponent->Model = Assets::FindStaticModel("WoodenBoat");
	RenderComponent->Model.Material = MaterialInstance::Create(m_DefaultMaterial);
	RenderComponent->Model.Material->AddTexture(Assets::FindTexture("WoodenBoatDiffuse"));

	auto* Collider = reinterpret_cast<ColliderComponent*>(AddComponent(m_BoatEntity, ComponentType::Collider));
	Collider->Weight = 200.0f;
	Collider->Velocity.x = 0.004f;
	Collider->IsAffectedByGravity = false;

	LevelScene->AddEntity(m_BoatEntity, { ComponentType::StaticRender, ComponentType::Collider });
}

void DemoLevel::CreateSailorEntity()
{
	m_SailorEntity = CreateEntity();
	auto* Transform = reinterpret_cast<TransformComponent*>(GetComponent(m_SailorEntity, ComponentType::Transform));
	Transform->Scale = { 3.6f, 3.6f, 3.6f };

	auto* RenderComponent = reinterpret_cast<AnimatedRenderComponent*>(AddComponent(m_SailorEntity, ComponentType::AnimatedRender));
	RenderComponent->Model = Assets::FindAnimatedModel("Sailor");
	RenderComponent->Model.material = MaterialInstance::Create(m_DefaultMaterial, ShaderGraphMeshType::MESH_ANIMATED);
	RenderComponent->Model.material->AddTexture(Assets::FindTexture("SailorDiffuse"));
	RenderComponent->Model.SetActiveAnimation("Armature|GreetingAnimation");
	RenderComponent->Model.GetActiveAnimation().ShouldLoopAnimation = false;
	RenderComponent->Model.GetAnimation("Armature|TalkingAnimation")->ShouldLoopAnimation = true;

	for (auto& animation : RenderComponent->Model.animations)
	{
		animation.AnimationSpeedDenominator = 120.0f;

		animation.ForEachBone(animation.GetRootBone(), [](Bone& bone) {
			glm::mat4 transform(1.0f);
			transform = glm::rotate(transform, glm::radians(-90.0f), { 0, 1, 0 });
			transform = glm::translate(transform, { -11.5f, 7.7f, -8.6f });
			bone.UserTransformation = transform;
		});
	}

	auto* Collider = reinterpret_cast<ColliderComponent*>(AddComponent(m_SailorEntity, ComponentType::Collider));
	Collider->Weight = 50.0f;
	Collider->Velocity.x = 0.004f;
	Collider->IsAffectedByGravity = false;

	LevelScene->AddEntity(m_SailorEntity, { ComponentType::AnimatedRender, ComponentType::Collider });
}

void DemoLevel::CreateSharkEntity()
{
	m_SharkEntity = CreateEntity();
	auto* Transform = reinterpret_cast<TransformComponent*>(GetComponent(m_SharkEntity, ComponentType::Transform));
	Transform->Position = { 640, -16, 640 };
	Transform->Rotation = { 0, glm::radians(-160.0f), 0 };
	Transform->Scale = { 0.4f, 0.4f, 0.4f };

	auto* RenderComponent = reinterpret_cast<AnimatedRenderComponent*>(AddComponent(m_SharkEntity, ComponentType::AnimatedRender));
	RenderComponent->Model = Assets::FindAnimatedModel("Shark");
	RenderComponent->Model.material = MaterialInstance::Create(m_DefaultMaterial, ShaderGraphMeshType::MESH_ANIMATED);
	RenderComponent->Model.material->AddTexture(Assets::FindTexture("SharkDiffuse"));
	RenderComponent->Model.SetActiveAnimation("Armature|Swimming.001");
	RenderComponent->Model.GetActiveAnimation().ShouldLoopAnimation = true;
	RenderComponent->Model.GetActiveAnimation().Play();

	for (auto& animation : RenderComponent->Model.animations)
	{
		animation.AnimationSpeedDenominator = 120.0f;
	}

	LevelScene->AddEntity(m_SharkEntity, { ComponentType::AnimatedRender });
}

void DemoLevel::CreateVegetationEntities()
{
	const auto& create_vegetation_entity = [this](glm::vec3 position, glm::vec3 rotation, float scale) {
		Entity vegetation = CreateEntity();
		auto* Transform = reinterpret_cast<TransformComponent*>(GetComponent(vegetation, ComponentType::Transform));
		Transform->Position = position;
		Transform->Rotation = rotation;
		Transform->Scale = { scale, scale, scale };

		auto* RenderComponent = reinterpret_cast<StaticRenderComponent*>(AddComponent(vegetation, ComponentType::StaticRender));
		RenderComponent->Model = Assets::FindStaticModel("Lotus");
		RenderComponent->Model.Material = MaterialInstance::Create(m_DefaultMaterial);
		RenderComponent->Model.Material->AddTexture(Assets::FindTexture("LotusDiffuse"));

		LevelScene->AddEntity(vegetation, { ComponentType::StaticRender });
	};

	create_vegetation_entity({ 170, -5, -170 }, { 0, 0, 0 }, 20);
	create_vegetation_entity({ 170, -5, 170 }, { 0, 2, 0 }, 20);
	create_vegetation_entity({ -30, -5, -170 }, { 0, 3, 0 }, 20);
	create_vegetation_entity({ -30, -5, 170 }, { 0, 1, 0 }, 20);
}
