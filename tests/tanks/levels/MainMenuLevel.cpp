#include "MainMenuLevel.h"

void MainMenuLevel::OnCreate()
{
	m_OrthoCamera = MakeRef<OrthographicCamera>();

	m_StartGameButton	= Assets::FindTexture("StartGameButton");
	m_QuitButton		= Assets::FindTexture("QuitButton");

	StartGameButtonPos	= { 256,  Window::Get().GetHeight() / 2 + 150 };
	QuitButtonPos		= { 256,  Window::Get().GetHeight() / 2 - 100 };

	ButtonSize = { 512, 254 };
}

void MainMenuLevel::OnRender()
{
	Renderer2D::BeginScene(*m_OrthoCamera);

	Renderer2D::DrawQuad(StartGameButtonPos,	ButtonSize, m_StartGameButton);
	Renderer2D::DrawQuad(QuitButtonPos,			ButtonSize, m_QuitButton);
	
	Renderer2D::EndScene();
}

void MainMenuLevel::OnEvent(Event& e)
{
	if (e.GetEventType() == EventType::MouseButtonReleased)
	{
		auto& evt = reinterpret_cast<MouseButtonReleasedEvent&>(e);

		if (evt.GetMouseButton() == MouseButton::Left)
			ProcessUIClickEvent(evt);
	}
}

void MainMenuLevel::ProcessUIClickEvent(MouseButtonReleasedEvent& e)
{
	auto [x, y] = Input::GetMousePosition();

	if (e.GetMouseButton() == MouseButton::Left)
	{
		// Create Lobby Button
		if (x > StartGameButtonPos.x - ButtonSize.x / 2 && x < StartGameButtonPos.x + ButtonSize.x / 2 - 50 &&
			y > (Window::Get().GetHeight() - StartGameButtonPos.y) - ButtonSize.y / 2 + 50 && y < (Window::Get().GetHeight() - StartGameButtonPos.y) + ButtonSize.y / 2 - 50)
		{
			LevelManager::Get().SetActiveLevel("Arena");
		}

		// Exit Button
		if (x > QuitButtonPos.x - ButtonSize.x / 2 && x < QuitButtonPos.x + ButtonSize.x / 2 - 50 &&
			y > (Window::Get().GetHeight() - QuitButtonPos.y) - ButtonSize.y / 2 + 50 && y < (Window::Get().GetHeight() - QuitButtonPos.y) + ButtonSize.y / 2 - 50)
		{
			IFRApplication::Get().Close();
		}
	}
}
