#pragma once
#include "Window/Window.h"
#include <Windows.h>
#include <rendering/RenderingContext.h>

namespace ifr
{
	class DirectXWindow;

	struct WindowCallbackData
	{
		DirectXWindow* p_EngineWindow = nullptr;
		Window::EventCallbackFn EventCallback;
	};

	class DirectXWindow : public Window
	{
	public:
		DirectXWindow(const WindowData& properties);
		~DirectXWindow();

		virtual void SetEventCallback(const EventCallbackFn& callback) override;

		virtual void Update() override;

		virtual unsigned int GetWidth() override;
		virtual unsigned int GetHeight() override;
		virtual void* GetNativeHandle() override { return (void*)m_NativeHandle; }

		virtual void SetCursorLocked(bool bLocked) override;
		bool IsCursorLocked() override;

		bool IsFocused() override;

		// Specific to DirectXWindow
		void OnResize(unsigned int width, unsigned int height);

	private:
		WindowData m_Properties;
		void Init();

		std::unique_ptr<RenderingContext> m_Context;

	private:
		HWND m_NativeHandle = nullptr;
		bool m_CursorLocked = false;

	private:
		WindowCallbackData m_CallbackData;
	};
}
