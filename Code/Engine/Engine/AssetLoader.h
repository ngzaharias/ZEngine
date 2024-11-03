#pragma once

#include "Core/Guid.h"
#include "Core/Map.h"
#include "Engine/TomlTypes.h"

namespace eng
{
	class AssetManager;
	struct Asset;
	struct AssetEntry;

	class AssetLoader
	{
		friend class eng::AssetManager;
		using Cache = Map<str::Guid, eng::Asset*>;

	public:
		AssetLoader() = default;
		virtual ~AssetLoader() = default;

		void ClearCache()
		{
			for (auto&& [guid, asset] : m_Cache)
				delete asset;
			m_Cache.RemoveAll();
		}

	protected:
		eng::AssetManager* m_AssetManager = nullptr;

		Cache m_Cache = { };
	};
}

#include "AssetLoader.h"