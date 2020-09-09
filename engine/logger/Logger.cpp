#include "Logger.h"
#include <Windows.h>
#include <ctime>
#include <rendering/ImGuiRenderer.h>

ifr::Logger _logger = ifr::Logger();

namespace ifr
{
	Logger::Logger() = default;

	void Logger::Log(const std::string & data)
	{
		if (OutputChannel == LoggerChannelType::STDOUT)
		{
			std::cout << GetTimeStamp() << "  " << data << "\n";
		}
		else if(OutputChannel == LoggerChannelType::IMGUI_CONSOLE)
		{
			auto str = GetTimeStamp() + "  " + data + "\n";
			ImGuiRenderer::AddToTextBuffer(str);
		}
	}

	std::string Logger::GetTimeStamp()
	{
		std::time_t t = std::time(0);
		std::tm now;
		localtime_s(&now, &t);
		return std::to_string(now.tm_year + 1900) + '-' + std::to_string(now.tm_mon + 1) + '-' + std::to_string(now.tm_mday) + ' ' +
			std::to_string(now.tm_hour) + ':' + std::to_string(now.tm_min) + ':' + std::to_string(now.tm_sec);
	}

	void Logger::SetConsoleTextColor(ConsoleTextColor color)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}

	Logger::~Logger()
	{
	}
}
