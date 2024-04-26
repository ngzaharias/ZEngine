#pragma once

#include <Core/Array.h>
#include <Core/Vector.h>

#include <Engine/Asset.h>
#include <Engine/AssetLoader.h>
#include <Engine/SpriteAsset.h>

namespace eng
{
	class Visitor;
}

namespace eng
{
	struct FlipbookFrame
	{
		Vector2f m_Position = Vector2f::Zero;
		Vector2f m_Size = Vector2f::Zero;
	};

	struct FlipbookAsset : public eng::Asset
	{
		str::Guid m_Shader = { };
		str::Guid m_Texture2D = { };
		float m_FPS = 30.f;
		bool m_IsLooping = true;

		Array<FlipbookFrame> m_Frames = { };
	};

	class FlipbookAssetLoader final : public eng::AssetLoader
	{
	public:
		bool Save(FlipbookAsset& asset, eng::Visitor& visitor) const;
		bool Load(FlipbookAsset& asset, eng::Visitor& visitor) const;
	};
}