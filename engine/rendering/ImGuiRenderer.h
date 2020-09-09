#pragma once
#include <core/IFRCore.h>
#include <string>
#include <vector>

struct ImGuiTextBuffer;

namespace ifr
{
	class Window;

	class ImGuiRenderer
	{
	public:
		static void Init(Window* window);
		static void Shutdown();

		static void OnWindowResize(unsigned int width, unsigned int height);

		static void BeginScene();
		static void EndScene();

		static void ShowDemoWindow();
		static void ShouldDisplayDebugInformation(bool show);
		static void ShouldUseCustomConsole(bool use);

		static void StartWindow(const std::string& title, uint32_t width = 0, uint32_t height = 0);
		static void EndWindow();

		static void BeginChild(const std::string& id, uint32_t width = 0, uint32_t height = 0);
		static void EndChild();

		static void NewLine();
		static void Separator();
		static void SameLine();
		static void Text(const std::string& text);
		static void SliderInt(const std::string& text, int& val, int min, int max);
		static void SliderFloat(const std::string& text, float& val, float min, float max);
		static bool Button(const std::string& text, float width = 0, float height = 0);
		static bool Combobox(const std::string& text, std::vector<std::string>& items, int& selected_index);
		static bool Checkbox(const std::string& text, bool& checked);
		static void InputText(const std::string& label, char* text, size_t buffersize);

		static void AddToTextBuffer(const std::string& text);

	private:
		static bool s_ShouldDisplayDebugInformation;
		static bool s_ShouldUseCustomConsole;
		static Ref<ImGuiTextBuffer> s_ImGuiTextBuffer;

		static void DisplayDebugInformation();
		static void DisplayCustomConsole();
	};
}
