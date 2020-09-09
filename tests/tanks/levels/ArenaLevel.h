#pragma once
#include "../scripts/PlayerControlScript.h"

class ArenaLevel : public Level
{
public:
	// Inherited via Level
	virtual void OnCreate() override;
	virtual void OnRender() override;
	virtual void OnEvent(Event& e) override;
	virtual void OnBecameActive() override;

private:
	Ref<OrthographicCamera> m_OrthoCamera;
	Ref<ThirdPersonCamera> m_ThirdPersonCamera;

	void ProcessUIMousePressEvent(MouseButtonPressedEvent& e);

private:
	StaticModel m_TankModel;
	Ref<Texture> m_HealthbarTextureFull;
	Ref<Texture> m_HealthbarTextureMid;
	Ref<Texture> m_HealthbarTextureLow;

	float m_MaxHealthbarVisibilityDistance = 1400.0f;

	bool m_bShouldHideCursor = true;

	Entity CreateTank();
	void DestroyTank(Entity tank);

	Ref<Texture> m_ExitGameTexture = nullptr;
	bool m_bPaused = false;
	void RenderPauseUI();

	void RenderTankHealthbars();

	void CreateScene();

	Entity PlayerTank;
	Entity OpponentTank;

private:
	Ref<Material> m_GlowMaterial;
	Ref<Material> m_DiffuseTextureMaterial;
	Ref<Material> m_NormalMappedTextureMaterial;

	void CreateMaterials();

	void CreateGlowMaterial();
	void CreateSimpleTextureMaterial();
	void CreateNormalMappedTextureMaterial();
};
