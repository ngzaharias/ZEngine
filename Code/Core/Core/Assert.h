#pragma once

#include "Log.h"

#ifdef ASSERTS_ENABLED

#include <cassert>

#define Z_ASSERT(condition, ...) \
{ \
	if (!(condition)) \
	{ \
		Z_LOG(ELog::Assert, __VA_ARGS__); \
	} \
}

#define Z_ASSERT_CRASH(condition, ...) \
{ \
	if (!(condition)) \
	{ \
		Z_LOG(ELog::Assert, __VA_ARGS__); \
		assert(false); \
	} \
}

#else

#define Z_ASSERT(condition, ...) {}
#define Z_ASSERT_CRASH(condition, ...) {}

#endif // ASSERTS_ENABLED
