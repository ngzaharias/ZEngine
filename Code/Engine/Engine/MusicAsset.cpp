#include "EnginePCH.h"
#include "Engine/MusicAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strSourceFile = NAME("m_SourceFile");
}

bool eng::MusicAssetLoader::Load(eng::MusicAsset& asset, eng::Visitor& visitor) const
{
	visitor.Read(strSourceFile, asset.m_SourceFile, {});
	return true;
}