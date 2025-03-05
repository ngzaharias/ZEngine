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
	bool WriteAsset(const char* label, str::Guid& value, const eng::AssetManager& manager);
}