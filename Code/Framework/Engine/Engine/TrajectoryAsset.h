#pragma once

#include "Core/Array.h"
#include "Engine/Asset.h"
#include "Engine/AssetLoader.h"
#include "Engine/SpriteAsset.h"
#include "Math/Vector.h"

namespace eng
{
	class Visitor;
}

namespace eng
{
	struct TrajectoryAsset : public eng::Asset
	{
		Array<Vector2f> m_Points = { };
	};

	class TrajectoryAssetLoader final : public eng::AssetLoader
	{
	public:
		bool Save(TrajectoryAsset& asset, eng::Visitor& visitor) const;
		bool Load(TrajectoryAsset& asset, eng::Visitor& visitor) const;
	};
}