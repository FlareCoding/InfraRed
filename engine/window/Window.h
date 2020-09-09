#pragma once
#include <core/IFRCore.h>
#include <events/Events.h>
#include <functional>

namespace ifr
{
	struct WindowData
	{
		unsigned int width;
		unsigned int height;
		const char* title;
		bool fullscreen;
		WindowData(bool fullscreen = false, unsigned int width = 1270, unsigned int height = 860, const char* title = "InfraRed Engine")
			: fullscreen(fullscreen), width(width), height(height), title(title) {}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		static Window& Get();

		static Window* Create(const WindowData& properties = WindowData());
		virtual ~Window() = default;

		virtual void Update() = 0;

		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual void* GetNativeHandle() = 0;

		virtual void SetCursorLocked(bool bLocked) = 0;
		virtual bool IsCursorLocked() = 0;

		virtual bool IsFocused() = 0;

	protected:
		static Window* s_WindowInstance;
	};
}