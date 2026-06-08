#pragma once
#include <string>
#include <mutex>

#define COLOR_RESET   "\033[0m"
#define COLOR_DEBUG   "\033[90m"
#define COLOR_INFO    "\033[37m"
#define COLOR_WARN    "\033[33m"
#define COLOR_ERROR   "\033[31m"
#define COLOR_FATAL   "\033[41m"

#define LOG_INFO(channel, msg) \
    cineris::log::Logger::info(channel, msg)

#define LOG_WARN(channel, msg) \
    cineris::log::Logger::warn(channel, msg)

#define LOG_ERROR(channel, msg) \
    cineris::log::Logger::error(channel, msg)

#define LOG_FATAL(channel, msg) \
    cineris::log::Logger::fatal(channel, msg)

#ifndef NDEBUG
#define LOG_DEBUG(channel, msg) \
			cineris::log::Logger::debug(channel, msg)
#else
#define LOG_DEBUG(channel, msg)
#endif

namespace cineris::log {
	enum class LogLevel {
		Debug,
		Info,
		Warn,
		Error,
		Fatal
	};

	enum class LogChannel {
		Engine,
		Game,
		Renderer,
		Input
	};

	auto enableConsoleColors() -> void;

	class Logger {
	public:
		static auto log(LogLevel level, LogChannel channel, const std::string& message) -> void;

		static auto debug(LogChannel channel, const std::string& message) -> void;
		static auto info(LogChannel channel, const std::string& message) -> void;
		static auto warn(LogChannel channel, const std::string& message) -> void;
		static auto error(LogChannel channel, const std::string& message) -> void;
		static auto fatal(LogChannel channel, const std::string& message) -> void;
	private:
		static auto levelToStr(LogLevel level) -> const char*;
		static auto channelToStr(LogChannel channel) -> const char*;
		static auto levelToColor(LogLevel level) -> const char*;


		static std::mutex s_Mutex;
	};
}