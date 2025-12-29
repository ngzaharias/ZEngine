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
		Vector2f m_Position = Vector2f::Zero;
		Vector2f m_Size = Vector2f::Zero;
	};

	class SpriteAssetLoader final : public eng::AssetLoader
	{
	public:
		void Bind(eng::SpriteAsset& asset) const;
		void Unbind(eng::SpriteAsset& asset) const;

		bool Load(eng::SpriteAsset& asset, eng::Visitor& visitor) const;
		bool Save(eng::SpriteAsset& asset, eng::Visitor& visitor) const;
	};
}