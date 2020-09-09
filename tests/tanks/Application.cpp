#include "Application.h"

void App::OnInitialize()
{
	m_LogoTexture = Texture2D::LoadFromFile("assets\\logo\\InfraRedLogo.png", TextureFormat::RGBA8, false);
	m_OrthoCamera = MakeRef<OrthographicCamera>();

	Assets::RegisterAssetOnLoadCallback([this](const std::string& name) {
		AssetOnLoad(name);
	});

	Assets::LoadTexture2D("assets\\textures\\StartGameButton.png",		"StartGameButton", false);
	Assets::LoadTexture2D("assets\\textures\\QuitButton.png",			"QuitButton", false);
	Assets::LoadStaticModel("assets\\models\\Cube.obj",					"Cube", false);
	Assets::LoadTexture2D("assets\\textures\\HealthbarFull.png",		"HealthbarFull", false);
	Assets::LoadTexture2D("assets\\textures\\HealthbarMid.png",			"HealthbarMid", false);
	Assets::LoadTexture2D("assets\\textures\\HealthbarLow.png",			"HealthbarLow", false);
	Assets::LoadTexture2D("assets\\textures\\TankDiffuse.jpg",			"TankDiffuse");
	Assets::LoadTexture2D("assets\\textures\\TankNormal.jpg",			"TankNormal");
	Assets::LoadStaticModel("assets\\models\\Tank.fbx",					"Tank", true);
	Assets::LoadTexture2D("assets\\textures\\ExitGame.png",				"ExitGameButton", false);
	Assets::LoadTexture2D("assets\\textures\\TronFloor.jpg",			"TronFloor");

	m_MainMenuLevel = MakeRef<MainMenuLevel>();
	m_ArenaLevel	= MakeRef<ArenaLevel>();
}

void App::OnUpdate(Timestep timestep)
{
	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	if (!m_AssetsLoaded)
		RenderLogoScreen();
}

void App::OnShutdown()
{
}

void App::OnEvent(Event& e)
{
}

void App::AssetOnLoad(const std::string& name)
{
	if (name._Equal("TronFloor"))
	{
		LevelManager::Get().RegisterLevel(m_MainMenuLevel, "MainMenu");
		LevelManager::Get().RegisterLevel(m_ArenaLevel, "Arena");

		LevelManager::Get().SetActiveLevel("MainMenu");
		m_AssetsLoaded = true;
	}
}

void App::RenderLogoScreen()
{
	Renderer2D::BeginScene(*m_OrthoCamera);
	Renderer2D::DrawQuad({ Window::Get().GetWidth() / 2.0f, Window::Get().GetHeight() / 2.0f }, { Window::Get().GetWidth(), Window::Get().GetHeight() }, m_LogoTexture);
	Renderer2D::EndScene();
}
