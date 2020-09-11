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
	double m_DemoStartTime = 0;

private:
	Ref<Material> m_DefaultMaterial = nullptr;
	Ref<Material> m_NormalMappedMaterial = nullptr;
	void CreateMaterials();

	Entity m_BoatEntity = INVALID_ENTITY;
	void CreateBoatEntity();

	Entity m_SailorEntity = INVALID_ENTITY;
	void CreateSailorEntity();
	bool m_StartedSailorAnimation = false;

	Entity m_SharkEntity = INVALID_ENTITY;
	void CreateSharkEntity();
	bool m_StartedSharkAnimation = false;
	bool m_StartedSharkJumpingAnimation = false;

	void CreateVegetationEntities();
};