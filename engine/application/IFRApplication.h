#pragma once
#include <core/IFRCore.h>
#include <time/Time.h>
#include <window/Window.h>
#include <input/Input.h>
#include <rendering/Renderer.h>
#include <camera/FPSCamera.h>
#include <camera/OrthographicCamera.h>
#include <camera/ThirdPersonCamera.h>
#include <camera/FreeRoamCamera.h>
#include <level/LevelManager.h>
#include <ecs/Components.h>
#include <asset_management/AssetManager.h>

namespace ifr
{
	struct ApplicationStartupSettings
	{
		bool Fullscreen = false;
		uint32_t WindowWidth  = 1270;
		uint32_t WindowHeight = 920;
		std::string WindowTitle = "InfraRed Engine";
	};

	class IFRApplication
	{
	public:
		static IFRApplication& Get() { return *s_Instance; }

		IFRApplication();
		~IFRApplication() = default;

		void Run();

		virtual void OnInitialize() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnImGuiDraw()  {}
		virtual void OnShutdown()   {}
		virtual void OnEvent(Event& e) {}

		inline void Close() { m_Running = false; }
		void SetApplicationStartupSettings(const ApplicationStartupSettings& settings) { m_StartupSettings = settings; }

	private:
		static IFRApplication* s_Instance;
		ApplicationStartupSettings m_StartupSettings;
		
		bool m_Running = false;

		UniqueRef<Window> m_Window;
		void InitializeWindow();

		UniqueRef<LevelManager> m_LevelManager;
		void InitializeLevelManager();

	private:
		void InternalEventHandler(Event& event);
	};
}