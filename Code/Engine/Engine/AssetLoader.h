#pragma once

namespace eng
{
	class AssetManager;

	class AssetLoader
	{
		friend class eng::AssetManager;

	public:
		AssetLoader() = default;
		virtual ~AssetLoader() = default;

	protected:
		eng::AssetManager* m_AssetManager = nullptr;
	};
}

#include "AssetLoader.h"