#pragma once

#include "Core/String.h"
#include "Core/Types.h"

namespace str
{
	using Hash = uint32;

	str::Hash ToHash(const char* value);
	str::Hash ToHash(const str::String& value);
}