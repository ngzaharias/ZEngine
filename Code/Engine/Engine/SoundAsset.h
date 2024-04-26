#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetLoader.h>

#include <SFML/Audio/SoundBuffer.hpp>

namespace eng
{
	class Visitor;
}

namespace eng
{
	struct SoundAsset : public eng::Asset
	{
		sf::SoundBuffer m_SoundBuffer = { };
		str::Path m_SourceFile = { };
	};

	class SoundAssetLoader final : public eng::AssetLoader
	{
	public:
		bool Load(SoundAsset& asset, eng::Visitor& visitor) const;
	};
}