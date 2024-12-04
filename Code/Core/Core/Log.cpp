#include "Core/Log.h"

#ifdef LOGGING_ENABLED

#include <stdarg.h>
#include <stdio.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#ifdef _WIN32
#include <windows.h>
#include <debugapi.h>
#else
#include <iostream>
#endif

namespace 
{
	static bool s_HasInitialised = false;
	std::shared_ptr<spdlog::logger> m_AssertLogger;
	std::shared_ptr<spdlog::logger> m_CrashLogger;
	std::shared_ptr<spdlog::logger> m_DebugLogger;
}

void core::LogInitialise()
{
	if (!s_HasInitialised)
	{
		s_HasInitialised = true;

		m_AssertLogger = spdlog::basic_logger_mt("assert", "asserts.txt");
		m_AssertLogger->set_level(spdlog::level::critical);

		m_CrashLogger = spdlog::basic_logger_mt("crash", "crashes.txt");
		m_CrashLogger->set_level(spdlog::level::critical);

		m_DebugLogger = spdlog::basic_logger_mt("debug", "debug.txt");
		m_DebugLogger->set_level(spdlog::level::info);
	}
}

void core::LogShutdown()
{
	if (s_HasInitialised)
	{
		s_HasInitialised = false;

		m_AssertLogger->flush();
		m_CrashLogger->flush();
		m_DebugLogger->flush();
	}
}

void core::LogMessage(const ELog channel, const char* message)
{
	core::LogInitialise();

#ifdef _WIN32
	OutputDebugStringA(message);
	OutputDebugStringA("\n");
#endif
	printf("%s\n", message);

	switch (channel)
	{
	case ELog::Assert:
		m_AssertLogger->critical(message);
		break;
	case ELog::Crash:
		m_CrashLogger->critical(message);
		m_CrashLogger->flush();
		break;

	default:
		m_DebugLogger->info(message);
		break;
	}
}

#endif