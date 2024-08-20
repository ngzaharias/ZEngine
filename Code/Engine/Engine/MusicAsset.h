#pragma once

#include "Core/Array.h"
#include "Engine/Asset.h"
#include "Engine/AssetLoader.h"

#include <SFML/Audio/Music.hpp>

namespace eng
{
	class Visitor;
}

namespace eng
{
	struct MusicAsset : public eng::Asset
	{
		sf::Music m_MusicBuffer = { };
		str::Path m_SourceFile = { };
	};

	class MusicAssetLoader final : public eng::AssetLoader
	{
	public:
		bool Load(eng::MusicAsset& asset, eng::Visitor& visitor) const;
	};
}