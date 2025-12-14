#pragma once

#include "Engine/AssetMethods.h"

namespace eng
{
	class AssetLoader;
	class AssetManager;
}

namespace str
{
	class Path;
}

namespace eng
{
	struct AssetEntry
	{
		eng::AssetLoader* m_Loader = nullptr;
		eng::AssetMethods m_Methods = {};

		using Load = void(eng::AssetManager&, const str::Path&);
		Load* m_Load = nullptr;
	};
}
