#pragma once

#include "Core/StringView.h"
#include "Core/Types.h"

namespace sort
{
	int32 AlphaNumeric(const str::StringView& a, const str::StringView& b);
}