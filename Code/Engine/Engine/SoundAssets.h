#pragma once

#include "Core/Array.h"
#include "Engine/Asset.h"
#include "Engine/AssetLoader.h"

#include <SFML/Audio/SoundBuffer.hpp>

namespace eng
{
	class Visitor;
}

namespace eng::sound
{
	struct RandomAsset : public eng::Asset
	{
		Array<str::Guid> m_Handles = {};
	};

	struct SequenceAsset : public eng::Asset
	{
		Array<str::Guid> m_Handles = {};
	};

	struct SingleAsset : public eng::Asset
	{
		// Buffers cannot be moved/deleted once they have started otherwise they will stop immediately.
		sf::SoundBuffer m_SoundBuffer = { };
		str::Path m_SourceFile = { };
	};

	class AssetLoader final : public eng::AssetLoader
	{
	public:
		bool Load(eng::sound::RandomAsset& asset, eng::Visitor& visitor) const;
		bool Load(eng::sound::SequenceAsset& asset, eng::Visitor& visitor) const;
		bool Load(eng::sound::SingleAsset& asset, eng::Visitor& visitor) const;
	};
}