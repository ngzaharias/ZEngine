#pragma once

#include "Core/Array.h"
#include "Icons/Icon.h"

namespace icon
{
#define ICON_DEFINITION(name, x, y) inline icon::Data name;
#include "Icons/IconList.h"
#undef ICON_DEFINITION

	inline Array<const icon::Data*> s_List;
}
