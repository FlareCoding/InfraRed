#pragma once
#include <core/IFRBuild.h>
#include <string>
#include <iostream>

namespace ifr
{
	enum ConsoleTextColor
	{
		CONSOLE_TEXT_COLOR_DEFAULT = 7,
		CONSOLE_TEXT_COLOR_RED = 4,
		CONSOLE_TEXT_COLOR_GREEN = 2,
		CONSOLE_TEXT_COLOR_BLUE = 1,
		CONSOLE_TEXT_COLOR_YELLOW = 14,
	};

	enum class LoggerChannelType
	{
		STDOUT,
		IMGUI_CONSOLE
	};

	class Logger
	{
	public:
		Logger();
		void Log(const std::string& data);
		void SetConsoleTextColor(ConsoleTextColor color);
		~Logger();

		LoggerChannelType OutputChannel = LoggerChannelType::STDOUT;

	private:
		std::string GetTimeStamp();
	};
}

extern ifr::Logger _logger;

#define IFR_LOG(data) _logger.Log(data);
#define IFR_LOG_FUNC(data) _logger.Log("" + std::string(__FUNCTION__) + " " + data);

#define IFR_LOG_ERROR(data)		{ _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_RED);		IFR_LOG(data); _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_DEFAULT); }
#define IFR_LOG_SUCCESS(data)	{ _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_GREEN);	IFR_LOG(data); _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_DEFAULT); }
#define IFR_LOG_STATUS(data)	{ _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_YELLOW);	IFR_LOG(data); _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_DEFAULT); }

#define IFR_LOG_FUNC_ERROR(data)	{ _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_RED);		IFR_LOG_FUNC(data); _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_DEFAULT); }
#define IFR_LOG_FUNC_SUCCESS(data)	{ _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_GREEN);	IFR_LOG_FUNC(data); _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_DEFAULT); }
#define IFR_LOG_FUNC_STATUS(data)	{ _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_YELLOW);	IFR_LOG_FUNC(data); _logger.SetConsoleTextColor(ifr::CONSOLE_TEXT_COLOR_DEFAULT); }