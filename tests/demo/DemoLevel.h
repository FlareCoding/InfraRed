#pragma once
#include <InfraRed.h>
using namespace ifr;

class DemoLevel : public Level
{
public:
	// Inherited via Level
	virtual void OnCreate() override;
	virtual void OnRender() override;
	virtual void OnEvent(Event& e) override;
	virtual void OnBecameActive() override;

private:
	Ref<OrthographicCamera> m_OrthoCamera;
	Ref<FreeRoamCamera> m_FreeCamera;
	void CreateScene();

	bool m_ShouldHideCursor = true;
	float m_InitialCoverOpacity = 2.0f;
private:
	Entity Boat = INVALID_ENTITY;
};