#pragma once
#include <string>
#include <mutex>
#include <format>
#include <utility>

#define COLOR_RESET   "\033[0m"
#define COLOR_DEBUG   "\033[90m"
#define COLOR_INFO    "\033[37m"
#define COLOR_WARN    "\033[33m"
#define COLOR_ERROR   "\033[31m"
#define COLOR_FATAL   "\033[41m"

#define LOG_INFO(channel, ...) \
    cineris::log::Logger::info(channel, __VA_ARGS__)

#define LOG_WARN(channel, ...) \
    cineris::log::Logger::warn(channel, __VA_ARGS__)

#define LOG_ERROR(channel, ...) \
    cineris::log::Logger::error(channel, __VA_ARGS__)

#define LOG_FATAL(channel, ...) \
    cineris::log::Logger::fatal(channel, __VA_ARGS__)

#ifndef NDEBUG
#define LOG_DEBUG(channel, ...) \
        cineris::log::Logger::debug(channel, __VA_ARGS__)
#else
#define LOG_DEBUG(channel, ...)
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
		template<typename... Args>
		static auto debug(LogChannel channel, std::format_string<Args...> fmt, Args&&... args) -> void {
			log(LogLevel::Debug, channel, std::format(fmt, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static auto info(LogChannel channel, std::format_string<Args...> fmt, Args&&... args) -> void {
			log(LogLevel::Info, channel, std::format(fmt, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static auto warn(LogChannel channel, std::format_string<Args...> fmt, Args&&... args) -> void {
			log(LogLevel::Warn, channel, std::format(fmt, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static auto error(LogChannel channel, std::format_string<Args...> fmt, Args&&... args) -> void {
			log(LogLevel::Error, channel, std::format(fmt, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static auto fatal(LogChannel channel, std::format_string<Args...> fmt, Args&&... args) -> void {
			log(LogLevel::Fatal, channel, std::format(fmt, std::forward<Args>(args)...));
		}

	private:
		static auto log(LogLevel level, LogChannel channel, const std::string& message) -> void;
		static auto levelToStr(LogLevel level) -> const char*;
		static auto channelToStr(LogChannel channel) -> const char*;
		static auto levelToColor(LogLevel level) -> const char*;


		static std::mutex s_Mutex;
	};
}