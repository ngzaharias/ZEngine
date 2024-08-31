#pragma once

#include "Log.h"

#ifdef ASSERTS_ENABLED

#include <cassert>

namespace core
{
	inline void ThrowUnhandledException()
	{
		int* ptr = nullptr; *ptr = 0;
	}
}

/// \brief Logs a message to file and output window (if attached to visual studio).
#define Z_ASSERT(condition, ...) \
{ \
	if (!(condition)) \
	{ \
		Z_LOG(ELog::Assert, __VA_ARGS__); \
	} \
}

/// \brief Logs a message to file and output window (if attached to visual studio), and then immediately crashes the program.
#define Z_PANIC(condition, ...) \
{ \
	if (!(condition)) \
	{ \
		Z_LOG(ELog::Crash, __VA_ARGS__); \
		core::ThrowUnhandledException(); \
	} \
}

#else

#define Z_ASSERT(condition, ...) {}
#define Z_PANIC(condition, ...) {}

#endif // ASSERTS_ENABLED
