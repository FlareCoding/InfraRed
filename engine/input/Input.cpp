#include "Input.h"
#include <window/Window.h>

namespace ifr
{
	bool Input::IsKeyPressed(int keycode)
	{
		if (!Window::Get().IsFocused()) return false;

		return s_InputInstance->IsKeyPressedImpl(keycode);
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		if (!Window::Get().IsFocused()) return false;

		return s_InputInstance->IsMouseButtonPressedImpl(button);
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		return s_InputInstance->GetMousePositionImpl();
	}

	float Input::GetMouseX()
	{
		return s_InputInstance->GetMouseXImpl();
	}

	float Input::GetMouseY()
	{
		return s_InputInstance->GetMouseYImpl();
	}
}
