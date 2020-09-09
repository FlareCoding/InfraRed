#pragma once
#include <core/IFRCore.h>

namespace ifr
{
	enum class MouseButton;

	class Input
	{
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(MouseButton button);

		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	protected:
		Input() = default;

		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(MouseButton button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static UniqueRef<Input> s_InputInstance;
	};
}
