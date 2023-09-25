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
		str::Guid m_Sprite = { };
		int32 m_FrameCount = 1;

		Vector2u m_Min = Vector2u::Zero;
		Vector2u m_Max = Vector2u::Zero;
	};

	struct FlipbookAsset : public eng::Asset
	{
		str::Guid m_Texture = { };
		float m_FPS = 30.f;
		bool m_IsLooping = true;

		Array<FlipbookFrame> m_Frames = { };
	};

	class FlipbookAssetLoader final : public eng::AssetLoader
	{
	public:
		bool Save(FlipbookAsset* asset, eng::Visitor& visitor) const;
		bool Load(FlipbookAsset* asset, eng::Visitor& visitor) const;
	};
}