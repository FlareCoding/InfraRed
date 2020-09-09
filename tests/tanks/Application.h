#pragma once
#include "scripts/PlayerControlScript.h"
#include "levels/ArenaLevel.h"
#include "levels/MainMenuLevel.h"

class App : public IFRApplication
{
public:
	void OnInitialize()		override;
	void OnUpdate(Timestep timestep) override;
	void OnShutdown()		override;
	void OnEvent(Event& e)	override;

	void AssetOnLoad(const std::string& name);

private:
	Ref<ArenaLevel>		m_ArenaLevel;
	Ref<MainMenuLevel>	m_MainMenuLevel;
	
	bool m_AssetsLoaded = false;

	Ref<OrthographicCamera> m_OrthoCamera;

private:
	Ref<Texture> m_LogoTexture;
	void RenderLogoScreen();
};
