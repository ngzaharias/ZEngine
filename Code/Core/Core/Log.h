#pragma once

enum class ELog
{
	Assert,
	Crash,
	Debug,
	Network,
};

namespace core
{
	void LogInitialise();
	void LogShutdown();

	template <typename... TArgs>
	inline void LogFormatted(const ELog channel, const char* format, TArgs&&... args);

	void LogMessage(const ELog channel, const char* message);
}

#ifdef LOGGING_ENABLED

#define Z_LOG(channel, message, ...) { core::LogFormatted(channel, message, __VA_ARGS__); }

#else

#define Z_LOG(channel, message, ...) {}

#endif // LOGS_ENABLED

#include "Log.inl"