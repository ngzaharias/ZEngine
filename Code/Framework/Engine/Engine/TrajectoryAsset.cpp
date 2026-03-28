#include "EnginePCH.h"
#include "Engine/TrajectoryAsset.h"

#include "Engine/AssetManager.h"
#include "Serialize/Visitor.h"

namespace
{
	const str::Name strPoints = NAME("m_Points");
}

bool eng::TrajectoryAssetLoader::Load(eng::TrajectoryAsset& asset, Visitor& visitor) const
{
	PROFILE_FUNCTION();
	visitor.Read(strPoints, asset.m_Points, {});
	return true;
}

bool eng::TrajectoryAssetLoader::Save(eng::TrajectoryAsset& asset, Visitor& visitor) const
{
	PROFILE_FUNCTION();
	visitor.Write(strPoints, asset.m_Points);
	return true;
}