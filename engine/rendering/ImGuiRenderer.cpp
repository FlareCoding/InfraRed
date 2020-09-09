#include "ImGuiRenderer.h"
#include <window/Window.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include <platform/DirectXResources.h>
#include <time/Time.h>

namespace ifr
{
	bool ImGuiRenderer::s_ShouldDisplayDebugInformation = false;
	bool ImGuiRenderer::s_ShouldUseCustomConsole = false;
	Ref<ImGuiTextBuffer> ImGuiRenderer::s_ImGuiTextBuffer = MakeRef<ImGuiTextBuffer>();

	void ImGuiRenderer::Init(Window* window)
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(window->GetNativeHandle());
		ImGui_ImplDX11_Init(DirectXResources::s_Device.Get(), DirectXResources::s_DeviceContext.Get());

		ImGui::StyleColorsDark();
	}

	void ImGuiRenderer::Shutdown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}
	
	void ImGuiRenderer::BeginScene()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (s_ShouldDisplayDebugInformation)
		{
			// Draw Debug Information
			DisplayDebugInformation();
		}

		if (s_ShouldUseCustomConsole)
		{
			// Draw Custom Console
			DisplayCustomConsole();
		}
	}
	
	void ImGuiRenderer::EndScene()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	
	void ImGuiRenderer::OnWindowResize(unsigned int width, unsigned int height)
	{
	}

	void ImGuiRenderer::ShowDemoWindow()
	{
		ImGui::Begin("IFR Demo Window");
		{
			static float f = 0.0f;
			static float col = 0.f;
			int show = 1;
			ImGui::Text(u8"Hello, world!");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)&col);
			if (ImGui::Button("Test Window")) show ^= 1;
			if (ImGui::Button("Another Window")) show ^= 1;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImVec2 v = ImGui::GetWindowSize();
		ImGui::Text("%f %f", v.x, v.y);
		ImGui::End();
	}
	
	void ImGuiRenderer::ShouldDisplayDebugInformation(bool show)
	{
		s_ShouldDisplayDebugInformation = show;
	}

	void ImGuiRenderer::ShouldUseCustomConsole(bool use)
	{
		s_ShouldUseCustomConsole = use;

		if (use)
		{
			_logger.OutputChannel = LoggerChannelType::IMGUI_CONSOLE;
		}
		else
		{
			_logger.OutputChannel = LoggerChannelType::STDOUT;
		}
	}

	void ImGuiRenderer::StartWindow(const std::string& title, uint32_t width, uint32_t height)
	{
		if (width != 0 && height != 0)
			ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));

		ImGui::Begin(title.c_str());
	}

	void ImGuiRenderer::EndWindow()
	{
		ImGui::End();
	}

	void ImGuiRenderer::BeginChild(const std::string& id, uint32_t width, uint32_t height)
	{
		ImGui::BeginChild(id.c_str(), ImVec2((float)width, (float)height));
	}

	void ImGuiRenderer::EndChild()
	{
		ImGui::EndChild();
	}

	void ImGuiRenderer::NewLine()
	{
		ImGui::NewLine();
	}

	void ImGuiRenderer::Separator()
	{
		ImGui::Separator();
	}

	void ImGuiRenderer::SameLine()
	{
		ImGui::SameLine();
	}

	void ImGuiRenderer::Text(const std::string& text)
	{
		ImGui::Text(text.c_str());
	}

	void ImGuiRenderer::SliderInt(const std::string& text, int& val, int min, int max)
	{
		ImGui::SliderInt(text.c_str(), &val, min, max);
	}

	void ImGuiRenderer::SliderFloat(const std::string& text, float& val, float min, float max)
	{
		ImGui::SliderFloat(text.c_str(), &val, min, max);
	}

	bool ImGuiRenderer::Button(const std::string& text, float width, float height)
	{
		return ImGui::Button(text.c_str(), ImVec2(width, height));
	}

	bool ImGuiRenderer::Combobox(const std::string& text, std::vector<std::string>& items, int& selected_index)
	{
		static auto vector_getter = [](void* vec, int idx, const char** out_text)
		{
			auto& vector = *static_cast<std::vector<std::string>*>(vec);
			if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
			*out_text = vector.at(idx).c_str();
			return true;
		};

		return ImGui::ListBox(text.c_str(), &selected_index, vector_getter, static_cast<void*>(&items), (int)items.size());
	}

	bool ImGuiRenderer::Checkbox(const std::string& text, bool& checked)
	{
		return ImGui::Checkbox(text.c_str(), &checked);
	}

	void ImGuiRenderer::InputText(const std::string& label, char* text, size_t buffersize)
	{
		ImGui::InputText(label.c_str(), &text[0], buffersize);
	}

	void ImGuiRenderer::AddToTextBuffer(const std::string& text)
	{
		if (s_ShouldDisplayDebugInformation)
		{
			s_ImGuiTextBuffer->appendf("%s", text.c_str());
		}
	}

	void ImGuiRenderer::DisplayDebugInformation()
	{
		static std::vector<float> frames;

		auto fps = ImGui::GetIO().Framerate;
		if (frames.size() > 40) // Max seconds to show
		{
			for (size_t i = 1; i < frames.size(); i++)
				frames[i - 1] = frames[i];

			frames[frames.size() - 1] = fps;
		}
		else
			frames.push_back(fps);

		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::Begin("FPS Graph", 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		char fps_text[60];
		sprintf_s(fps_text, 60, "Frames: %f", fps);
		ImGui::Text(fps_text);

		char timestep_text[26];
		sprintf_s(timestep_text, 26, "Timestep: %f ms", Time::GetTimestep());
		ImGui::Text(timestep_text);

		ImGui::PlotHistogram("Framerate", &frames[0], (int)frames.size(), 0, NULL, 0.0f, 600, ImVec2(300, 100));
		ImGui::End();
	}
	
	void ImGuiRenderer::DisplayCustomConsole()
	{
		ImGui::SetNextWindowPos(ImVec2(10, (float)Window::Get().GetHeight() - 362));
		ImGui::SetNextWindowSize(ImVec2((float)Window::Get().GetWidth() * (2 / 5), 360));
		ImGui::Begin("Console");
		
		ImGui::TextUnformatted(s_ImGuiTextBuffer->Buf.begin());
		ImGui::SetScrollHere(1.0f);

		ImGui::End();
	}
}
