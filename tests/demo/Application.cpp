#include "Application.h"

void Demo::OnInitialize()
{
	Window::Get().SetCursorLocked(true);
	m_OrthoCamera = MakeRef<OrthographicCamera>();
	m_DemoLevel = MakeRef<DemoLevel>();
	m_LogoTexture = Texture2D::LoadFromFile("assets\\logo\\InfraRedLogo.png", TextureFormat::RGBA8, false);

	Assets::RegisterAssetOnLoadCallback([this](const std::string& name) {
		AssetOnLoad(name);
	});

	Assets::LoadAudioSource("OceanWaves.wav", "OceanWaves");
	Assets::LoadAudioSource("Greeting.wav", "Greeting");
	Assets::LoadAudioSource("Introduction.wav", "Introduction");
	Assets::LoadAudioSource("LookAShark.wav", "LookAShark");
	Assets::LoadAudioSource("SlowMotion.wav", "SlowMotion");
	Assets::LoadTexture2D("WoodenBoat.jpg", "WoodenBoatDiffuse");
	Assets::LoadStaticModel("WoodenBoat.fbx", "WoodenBoat", true);
	Assets::LoadTexture2D("Lumberjack.png", "SailorDiffuse");
	Assets::LoadAnimatedModel("Sailor.fbx", "Sailor", true);
	Assets::LoadTexture2D("Rock.jpg", "RockDiffuse", true);
	Assets::LoadTexture2D("RockNormal.png", "RockNormal", true);
	Assets::LoadStaticModel("Rock.obj", "Rock", false);
	Assets::LoadTexture2D("LilyPad.png", "LilyPadDiffuse", true);
	Assets::LoadStaticModel("LilyPad.obj", "LilyPad", false);
	Assets::LoadTexture2D("Lotus.jpg", "LotusDiffuse", true);
	Assets::LoadStaticModel("Lotus.obj", "Lotus", false);
	Assets::LoadTexture2D("Shark.png", "SharkDiffuse", true);
	Assets::LoadAnimatedModel("Shark.fbx", "Shark", false);
	Assets::LoadTextureCube(
		{
			("skyboxes\\SB1_right.png"),
			("skyboxes\\SB1_left.png"),
			("skyboxes\\SB1_top.png"),
			("skyboxes\\SB1_bottom.png"),
			("skyboxes\\SB1_back.png"),
			("skyboxes\\SB1_front.png")
		},
		"DemoSkybox"
	);
}

void Demo::OnUpdate(Timestep timestep)
{
	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));

	if (!m_AssetsLoaded)
		RenderLogoScreen();
}

void Demo::OnShutdown()
{
}

void Demo::OnEvent(Event& e)
{
}

void Demo::AssetOnLoad(const std::string& name)
{
	if (name._Equal("DemoSkybox"))
	{
		LevelManager::Get().RegisterLevel(m_DemoLevel, "DemoLevel");
		LevelManager::Get().SetActiveLevel("DemoLevel");
		m_AssetsLoaded = true;
	}
}

void Demo::RenderLogoScreen()
{
	Renderer2D::BeginScene(*m_OrthoCamera);
	Renderer2D::DrawQuad({ Window::Get().GetWidth() / 2.0f, Window::Get().GetHeight() / 2.0f }, { Window::Get().GetWidth(), Window::Get().GetHeight() }, m_LogoTexture);
	Renderer2D::EndScene();
}
