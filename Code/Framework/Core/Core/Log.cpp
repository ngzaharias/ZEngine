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
	std::shared_ptr<spdlog::logger> m_AssertLogger;
	std::shared_ptr<spdlog::logger> m_CrashLogger;
	std::shared_ptr<spdlog::logger> m_DebugLogger;
	std::shared_ptr<spdlog::logger> m_NetworkLogger;
}

void core::LogInitialise()
{
	m_AssertLogger = spdlog::basic_logger_mt("assert", "asserts.txt");
	m_AssertLogger->set_level(spdlog::level::critical);

	m_CrashLogger = spdlog::basic_logger_mt("crash", "crashes.txt");
	m_CrashLogger->set_level(spdlog::level::critical);

	m_DebugLogger = spdlog::basic_logger_mt("debug", "debug.txt");
	m_DebugLogger->set_level(spdlog::level::info);

	m_NetworkLogger = spdlog::basic_logger_mt("network", "network.txt");
	m_NetworkLogger->set_level(spdlog::level::info);
}

void core::LogShutdown()
{
	m_AssertLogger->flush();
	m_CrashLogger->flush();
	m_DebugLogger->flush();
	m_NetworkLogger->flush();
}

void core::LogMessage(const ELog channel, const char* message)
{
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
	case ELog::Debug:
		m_DebugLogger->info(message);
		break;
	case ELog::Error:
		m_DebugLogger->error(message);
		break;
	case ELog::Warn:
		m_DebugLogger->warn(message);
		break;

		// Customs
	case ELog::Network:
		m_NetworkLogger->warn(message);
		break;
	}
}

#endif