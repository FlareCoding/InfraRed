#pragma once
#include <InfraRed.h>
using namespace ifr;

class MainMenuLevel : public Level
{
public:
	// Inherited via Level
	virtual void OnCreate() override;
	virtual void OnRender() override;
	virtual void OnEvent(Event& e) override;

private:
	Ref<OrthographicCamera> m_OrthoCamera;

	Ref<Texture> m_StartGameButton;
	Ref<Texture> m_QuitButton;

	void ProcessUIClickEvent(MouseButtonReleasedEvent& e);

	glm::vec2 StartGameButtonPos, QuitButtonPos;
	glm::vec2 ButtonSize;
};
