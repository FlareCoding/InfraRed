#pragma once
#include <InfraRed.h>
using namespace ifr;

class SandboxApp : public IFRApplication
{
public:
	void OnInitialize() override;
	void OnUpdate(Timestep timestep) override;
	void OnImGuiDraw()  override;
	void OnShutdown()   override;
	void OnEvent(Event& e) override;

private:
	 Ref<OrthographicCamera> m_OrthoCamera;
	 Ref<ThirdPersonCamera> m_ThirdPersonCamera;
	 Ref<FreeRoamCamera> m_FreeCamera;

	 Light m_Sun;

private:
	void AssetOnLoad(const std::string& name);

private:
	void CreateMaterials();

	void CreateGlowMaterial();
	void CreateSimpleTextureMaterial();
	void CreateNormalMappedTextureMaterial();
};
