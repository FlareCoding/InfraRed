#pragma once
#include "DemoLevel.h"

class Demo : public IFRApplication
{
public:
	void OnInitialize()		override;
	void OnUpdate(Timestep timestep) override;
	void OnShutdown()		override;
	void OnEvent(Event& e)	override;

	void AssetOnLoad(const std::string& name);

private:
	bool m_AssetsLoaded = false;

	Ref<OrthographicCamera> m_OrthoCamera;
	Ref<DemoLevel>	m_DemoLevel;

private:
	Ref<Texture> m_LogoTexture;
	void RenderLogoScreen();
};
