#pragma once
#include "Event.h"

namespace ifr
{
	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		REGISTER_EVT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(KeyReleased)
	};
}

// =========================================================================================== //
// =========================================================================================== //
// ======================================== KEY CODES ======================================== //
// =========================================================================================== //
// =========================================================================================== //

#define IFR_KEY_SPACE              32
#define IFR_KEY_APOSTROPHE         39  /* ' */
#define IFR_KEY_COMMA              44  /* , */
#define IFR_KEY_MINUS              45  /* - */
#define IFR_KEY_PERIOD             46  /* . */
#define IFR_KEY_SLASH              47  /* / */
#define IFR_KEY_0                  48
#define IFR_KEY_1                  49
#define IFR_KEY_2                  50
#define IFR_KEY_3                  51
#define IFR_KEY_4                  52
#define IFR_KEY_5                  53
#define IFR_KEY_6                  54
#define IFR_KEY_7                  55
#define IFR_KEY_8                  56
#define IFR_KEY_9                  57
#define IFR_KEY_SEMICOLON          59  /* ; */
#define IFR_KEY_EQUAL              61  /* = */
#define IFR_KEY_A                  65
#define IFR_KEY_B                  66
#define IFR_KEY_C                  67
#define IFR_KEY_D                  68
#define IFR_KEY_E                  69
#define IFR_KEY_F                  70
#define IFR_KEY_G                  71
#define IFR_KEY_H                  72
#define IFR_KEY_I                  73
#define IFR_KEY_J                  74
#define IFR_KEY_K                  75
#define IFR_KEY_L                  76
#define IFR_KEY_M                  77
#define IFR_KEY_N                  78
#define IFR_KEY_O                  79
#define IFR_KEY_P                  80
#define IFR_KEY_Q                  81
#define IFR_KEY_R                  82
#define IFR_KEY_S                  83
#define IFR_KEY_T                  84
#define IFR_KEY_U                  85
#define IFR_KEY_V                  86
#define IFR_KEY_W                  87
#define IFR_KEY_X                  88
#define IFR_KEY_Y                  89
#define IFR_KEY_Z                  90
#define IFR_KEY_LEFT_BRACKET       91  /* [ */
#define IFR_KEY_BACKSLASH          92  /* \ */
#define IFR_KEY_RIGHT_BRACKET      93  /* ] */
#define IFR_KEY_GRAVE_ACCENT       96  /* ` */
#define IFR_KEY_WORLD_1            161 /* non-US #1 */
#define IFR_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define IFR_KEY_ESCAPE             0x1B
#define IFR_KEY_ENTER              0x0D
#define IFR_KEY_TAB                0x09
#define IFR_KEY_BACKSPACE          0x08
#define IFR_KEY_INSERT             0x2D
#define IFR_KEY_DELETE             0x2E
#define IFR_KEY_RIGHT              0x27
#define IFR_KEY_LEFT               0x25
#define IFR_KEY_DOWN               0x28
#define IFR_KEY_UP                 0x26
#define IFR_KEY_PAGE_UP            266
#define IFR_KEY_PAGE_DOWN          267
#define IFR_KEY_HOME               268
#define IFR_KEY_END                269
#define IFR_KEY_CAPS_LOCK          0x14
#define IFR_KEY_SCROLL_LOCK        281
#define IFR_KEY_NUM_LOCK           282
#define IFR_KEY_PRINT_SCREEN       283
#define IFR_KEY_PAUSE              284
#define IFR_KEY_F1                 290
#define IFR_KEY_F2                 291
#define IFR_KEY_F3                 292
#define IFR_KEY_F4                 293
#define IFR_KEY_F5                 294
#define IFR_KEY_F6                 295
#define IFR_KEY_F7                 296
#define IFR_KEY_F8                 297
#define IFR_KEY_F9                 298
#define IFR_KEY_F10                299
#define IFR_KEY_F11                300
#define IFR_KEY_F12                301
#define IFR_KEY_F13                302
#define IFR_KEY_F14                303
#define IFR_KEY_F15                304
#define IFR_KEY_F16                305
#define IFR_KEY_F17                306
#define IFR_KEY_F18                307
#define IFR_KEY_F19                308
#define IFR_KEY_F20                309
#define IFR_KEY_F21                310
#define IFR_KEY_F22                311
#define IFR_KEY_F23                312
#define IFR_KEY_F24                313
#define IFR_KEY_F25                314
#define IFR_KEY_KP_0               320
#define IFR_KEY_KP_1               321
#define IFR_KEY_KP_2               322
#define IFR_KEY_KP_3               323
#define IFR_KEY_KP_4               324
#define IFR_KEY_KP_5               325
#define IFR_KEY_KP_6               326
#define IFR_KEY_KP_7               327
#define IFR_KEY_KP_8               328
#define IFR_KEY_KP_9               329
#define IFR_KEY_KP_DECIMAL         330
#define IFR_KEY_KP_DIVIDE          331
#define IFR_KEY_KP_MULTIPLY        332
#define IFR_KEY_KP_SUBTRACT        333
#define IFR_KEY_KP_ADD             334
#define IFR_KEY_KP_ENTER           335
#define IFR_KEY_KP_EQUAL           336
#define IFR_KEY_LEFT_SHIFT         0xA0
#define IFR_KEY_LEFT_CONTROL       17
#define IFR_KEY_LEFT_ALT           0x12
#define IFR_KEY_LEFT_SUPER         343
#define IFR_KEY_RIGHT_SHIFT        0xA1
#define IFR_KEY_RIGHT_CONTROL      0xA3
#define IFR_KEY_RIGHT_ALT          0xA5
#define IFR_KEY_RIGHT_SUPER        347
#define IFR_KEY_MENU               348