#include <cineris/core/logger.h>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace cineris::log {
	std::mutex Logger::s_Mutex;

	auto enableConsoleColors() -> void {
#ifdef _WIN32
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		if (hOut == INVALID_HANDLE_VALUE)
			return;

		DWORD mode = 0;

		if (!GetConsoleMode(hOut, &mode))
			return;

		mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, mode);
#endif
	}

	auto Logger::log(LogLevel level, LogChannel channel, const std::string& message) -> void {
		std::lock_guard<std::mutex> lock(s_Mutex);

		const char* color = levelToColor(level);
		
		std::cout
			<< color
			<< "["
			<< channelToStr(channel)
			<< "]"
			<< "["
			<< levelToStr(level)
			<< "] "
			<< message
			<< COLOR_RESET
			<< std::endl;


	}

	auto Logger::levelToStr(LogLevel level) -> const char* {
		switch (level) {
		case LogLevel::Debug:
			return "DEBUG";
		case LogLevel::Info:
			return "INFO";
		case LogLevel::Warn:
			return "WARNING";
		case LogLevel::Error:
			return "ERROR";
		case LogLevel::Fatal:
			return "FATAL";
		}

		return "UNKNOWN";
	}

	auto Logger::channelToStr(LogChannel channel) -> const char* {
		switch (channel) {
		case LogChannel::Engine:
			return "ENGINE";
		case LogChannel::Game:
			return "GAME";
		case LogChannel::Renderer:
			return "RENDERER";
		case LogChannel::Input:
			return "INPUT";
		}

		return "UNKNOWN";
	}

	auto Logger::levelToColor(LogLevel level) -> const char* {
		switch (level)
		{
		case LogLevel::Debug: return COLOR_DEBUG; // gray
		case LogLevel::Info:  return COLOR_INFO; // white
		case LogLevel::Warn:  return COLOR_WARN; // yellow
		case LogLevel::Error: return COLOR_ERROR; // red
		case LogLevel::Fatal: return COLOR_FATAL; // red bg
		}

		return COLOR_RESET;
	}
}