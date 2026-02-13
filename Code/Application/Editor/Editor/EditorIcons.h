#pragma once

#include "Core/Array.h"
#include "Editor/EditorIcon.h"

namespace icon
{
#define ICON_DEFINITION(name, x, y) inline editor::Icon name;
#include "Editor/EditorIconList.h"
#undef ICON_DEFINITION

	inline Array<const editor::Icon*> s_List;
}
