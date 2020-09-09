#include "Application.h"

void Demo::OnInitialize()
{
	m_OrthoCamera = MakeRef<OrthographicCamera>();
	m_DemoLevel = MakeRef<DemoLevel>();
	m_LogoTexture = Texture2D::LoadFromFile("assets\\logo\\InfraRedLogo.png", TextureFormat::RGBA8, false);

	Assets::RegisterAssetOnLoadCallback([this](const std::string& name) {
		AssetOnLoad(name);
	});

	Assets::LoadTexture2D("assets\\textures\\WoodenBoat.jpg", "WoodenBoatDiffuse");
	Assets::LoadStaticModel("assets\\models\\WoodenBoat.fbx", "WoodenBoat", true);
	Assets::LoadTextureCube(
		{
			("assets\\textures\\skyboxes\\SB1_right.png"),
			("assets\\textures\\skyboxes\\SB1_left.png"),
			("assets\\textures\\skyboxes\\SB1_top.png"),
			("assets\\textures\\skyboxes\\SB1_bottom.png"),
			("assets\\textures\\skyboxes\\SB1_back.png"),
			("assets\\textures\\skyboxes\\SB1_front.png")
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
