#include "EnginePCH.h"
#include "Engine/SoundAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strSourceFile = NAME("m_SourceFile");
}

bool eng::SoundAssetLoader::Load(SoundAsset& asset, eng::Visitor& visitor) const
{
	visitor.Visit(strSourceFile, asset.m_SourceFile, {});

	// #todo: error message
	// #todo: fallback to default asset
	if (asset.m_SourceFile.IsEmpty())
		return false;

	const str::Path filepath = str::Path(str::EPath::Assets, asset.m_SourceFile);
	asset.m_SoundBuffer.loadFromFile(asset.m_SourceFile.ToChar());
	return true;
}