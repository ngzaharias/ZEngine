#pragma once

#ifdef USE_OPTICK

#include <optick.h>

#define PROFILE_CUSTOM(...) OPTICK_EVENT(__VA_ARGS__)
#define PROFILE_FUNCTION() OPTICK_EVENT()
#define PROFILE_TICK(name) OPTICK_FRAME(name)

namespace profile
{
	inline bool IsActive() { return ::Optick::IsActive(); }
}

#else

#define PROFILE_FUNCTION()
#define PROFILE_TICK(name)

namespace profile
{
	inline bool IsActive() { return false; }
}

#endif