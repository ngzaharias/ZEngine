#pragma once

#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/Types.h"

namespace str
{
	using Hash = uint32;

	str::Hash ToHash(const str::StringView& value);
}