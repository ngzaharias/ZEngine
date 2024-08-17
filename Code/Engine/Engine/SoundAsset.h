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
	struct RandomContainer
	{
	};

	struct SequenceContainer
	{
	};

	struct RandomAsset : public eng::Asset
	{
		Array<str::Guid> m_Sounds = {};
	};

	struct SequenceAsset : public eng::Asset
	{
		Array<str::Guid> m_Sounds = {};
	};

	struct SourceAsset : public eng::Asset
	{
		sf::SoundBuffer m_SoundBuffer = { };
		str::Path m_SourceFile = { };
	};

	class AssetLoader final : public eng::AssetLoader
	{
	public:
		bool Load(eng::sound::RandomAsset& asset, eng::Visitor& visitor) const;
		bool Load(eng::sound::SequenceAsset& asset, eng::Visitor& visitor) const;
		bool Load(eng::sound::SourceAsset& asset, eng::Visitor& visitor) const;
	};
}