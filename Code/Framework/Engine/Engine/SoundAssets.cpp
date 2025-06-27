#include "EnginePCH.h"
#include "Engine/SoundAssets.h"

#include "Engine/AssetManager.h"
#include "Engine/Visitor.h"

#include <SFML/Audio/SoundBuffer.hpp>

namespace
{
	const str::Name strHandles = NAME("m_Handles");
	const str::Name strSourceFile = NAME("m_SourceFile");
}

bool eng::sound::AssetLoader::Load(eng::sound::RandomAsset& asset, eng::Visitor& visitor) const
{
	visitor.Read(strHandles, asset.m_Handles, {});
	return true;
}

bool eng::sound::AssetLoader::Load(eng::sound::SequenceAsset& asset, eng::Visitor& visitor) const
{
	visitor.Read(strHandles, asset.m_Handles, {});
	return true;
}

bool eng::sound::AssetLoader::Load(eng::sound::SingleAsset& asset, eng::Visitor& visitor) const
{
	visitor.Read(strSourceFile, asset.m_SourceFile, {});

	// #todo: error message
	// #todo: fallback to default asset
	if (asset.m_SourceFile.IsEmpty())
		return false;

	const str::Path filepath = str::Path(str::EPath::Assets, asset.m_SourceFile);
	asset.m_SoundBuffer.loadFromFile(filepath.ToChar());
	return true;
}