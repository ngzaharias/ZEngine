#pragma once

#include "Core/Guid.h"
#include "Engine/Asset.h"
#include "Engine/AssetLoader.h"
#include "Math/Vector.h"

namespace eng
{
	class Visitor;
}

namespace eng
{
	struct SpriteAsset : public eng::Asset
	{
		str::Guid m_Shader = { };
		str::Guid m_Texture2D = { };
		Vector2u m_Position = Vector2u::Zero;
		Vector2u m_Size = Vector2u::Zero;
	};

	class SpriteAssetLoader final : public eng::AssetLoader
	{
	public:
		bool Save(SpriteAsset& asset, eng::Visitor& visitor) const;
		bool Load(SpriteAsset& asset, eng::Visitor& visitor) const;
	};
}