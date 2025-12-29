#include "EnginePCH.h"
#include "Engine/TrajectoryAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/TomlHelpers.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strPoints = NAME("m_Points");
}

bool eng::TrajectoryAssetLoader::Load(eng::TrajectoryAsset& asset, eng::Visitor& visitor) const
{
	visitor.Read(strPoints, asset.m_Points, {});
	return true;
}

bool eng::TrajectoryAssetLoader::Save(eng::TrajectoryAsset& asset, eng::Visitor& visitor) const
{
	visitor.Write(strPoints, asset.m_Points);
	return true;
}