#include "DirectXInput.h"
#include <window/Window.h>
#include <events/MouseEvents.h>
#include <Windows.h>

namespace ifr
{
	static int GetWindowsKeyCodeFromSEKeycode(int se_keycode)
	{
		switch (se_keycode)
		{
		case IFR_KEY_SPACE: return VK_SPACE;
		case IFR_KEY_ESCAPE: return VK_ESCAPE;
		case IFR_KEY_ENTER: return VK_RETURN;
		case IFR_KEY_TAB: return VK_TAB;
		case IFR_KEY_BACKSPACE: return VK_BACK;
		case IFR_KEY_INSERT: return VK_INSERT;
		case IFR_KEY_DELETE: return VK_DELETE;
		case IFR_KEY_LEFT: return VK_LEFT;
		case IFR_KEY_RIGHT: return VK_RIGHT;
		case IFR_KEY_UP: return VK_UP;
		case IFR_KEY_DOWN: return VK_DOWN;
		case IFR_KEY_LEFT_CONTROL: return VK_LCONTROL;
		case IFR_KEY_RIGHT_CONTROL: return VK_RCONTROL;

		default: return se_keycode;
		}

		return 0;;
	}

	UniqueRef<Input> Input::s_InputInstance = UniqueRef<Input>(new DirectXInput());

	bool DirectXInput::IsKeyPressedImpl(int keycode)
	{

		return (GetKeyState(GetWindowsKeyCodeFromSEKeycode(keycode)) & 0x8000);
	}

	bool DirectXInput::IsMouseButtonPressedImpl(MouseButton button)
	{
		if		(button == MouseButton::Left) return (GetKeyState(VK_LBUTTON) & 0x8000);
		else if (button == MouseButton::Right) return (GetKeyState(VK_RBUTTON) & 0x8000);
		else if (button == MouseButton::Middle) return (GetKeyState(VK_MBUTTON) & 0x8000);
		return false;
	}

	std::pair<float, float> DirectXInput::GetMousePositionImpl()
	{
		HWND hwnd = (HWND)Window::Get().GetNativeHandle();
		POINT pos;
		GetCursorPos(&pos);
		if (hwnd)
		{
			ScreenToClient(hwnd, &pos);
		}
		return { (float)pos.x, (float)pos.y };
	}
	
	float DirectXInput::GetMouseXImpl()
	{
		auto pos = GetMousePositionImpl();
		return pos.first;
	}
	
	float DirectXInput::GetMouseYImpl()
	{
		auto pos = GetMousePositionImpl();
		return pos.second;
	}
}
