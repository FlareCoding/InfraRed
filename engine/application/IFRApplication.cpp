#include "IFRApplication.h"

namespace ifr
{
#define BIND_APPLICATION_EVENT_FN(x) std::bind(&IFRApplication::x, this, std::placeholders::_1)

	IFRApplication* IFRApplication::s_Instance = nullptr;

	IFRApplication::IFRApplication()
	{
		s_Instance = this;
	}

	void IFRApplication::Run()
	{
		m_Running = true;
		InitializeWindow();
		InitializeLevelManager();

		OnInitialize();

		// Main Rendering Thread
		while (m_Running)
		{
			Time::FrameUpdate();

			OnUpdate(Time::GetTimestep());

			m_LevelManager->OnRender();

			ImGuiRenderer::BeginScene();
			OnImGuiDraw();
			ImGuiRenderer::EndScene();

			m_Window->Update();
		}

		OnShutdown();
	}

	void IFRApplication::InitializeWindow()
	{
		 m_Window = UniqueRef<Window>(Window::Create(WindowData(
		 						m_StartupSettings.Fullscreen, 
		 						m_StartupSettings.WindowWidth, 
		 						m_StartupSettings.WindowHeight, 
		 						m_StartupSettings.WindowTitle.c_str())));

		 m_Window->SetEventCallback(BIND_APPLICATION_EVENT_FN(InternalEventHandler));

		 Renderer::Init(m_Window.get());
	}

	void IFRApplication::InitializeLevelManager()
	{
		m_LevelManager = UniqueRef<LevelManager>(new LevelManager());
	}

	void IFRApplication::InternalEventHandler(Event& event)
	{
		 // Drop certain input events if the window is not in focus
		 if (!m_Window->IsFocused() && event.IsInCategory(EventCategory::EventCategoryInput))
		 	return;

		 OnEvent(event);
		 m_LevelManager->OnEvent(event);

		 if (event.GetEventType() == EventType::WindowClose)
		 {
		 	Close();
		 }
		 else if (event.GetEventType() == EventType::WindowResize)
		 {
			WindowResizeEvent& wre = static_cast<WindowResizeEvent&>(event);
		 	ImGuiRenderer::OnWindowResize(wre.GetWidth(), wre.GetHeight());
		 }
	}
}
