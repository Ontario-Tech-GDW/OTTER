#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/Logger.h"

class LoggerBase {
public:
	struct LoggerSettings
	{
		bool OutputToFile;
		bool OutputToConsole;
		std::string LogFileName;
		LoggerSettings() :
			OutputToFile(false), OutputToConsole(true), LogFileName("logs.txt") {}
	};
	/*
		Initializes the logging subsystem, and sets up the color logger and debug trace utilities
	*/
	static void Init(const LoggerSettings& settings = LoggerSettings());

	/*
		De-initializes the logging subsytem, and cleans up all the logging resources
	 */
	static void Uninitialize();

	/*
		Gets the logging instance
	*/
	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return myLogger; }
	/*
		Dumps the current stack trace into a string for logging
	*/
	static std::string DumpStackTrace();

private:
	static std::shared_ptr<spdlog::logger> myLogger;
	static bool isInitialized;
};

// Client log macros
#define LOG_TRACE(...) ::LoggerBase::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)  ::LoggerBase::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  ::LoggerBase::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) { ::LoggerBase::GetLogger()->error(__VA_ARGS__); ::LoggerBase::GetLogger()->error("Location: \n{}", ::LoggerBase::DumpStackTrace()); }

// Allows us to assert if a value is true, and automagically debug break if it is false
#define LOG_ASSERT(x, ...) { if (!(x)) { ::LoggerBase::GetLogger()->error(__VA_ARGS__); __debugbreak(); } }