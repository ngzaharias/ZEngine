#pragma once

#include "imgui/Inspector.h"

namespace eng
{
	class AssetManager;
}

namespace str
{
	class Guid;
}

namespace imgui
{
	bool WriteSprite(const eng::AssetManager& manager, const char* label, str::Guid& value);
	bool WriteTexture2D(const eng::AssetManager& manager, const char* label, str::Guid& value);
}
