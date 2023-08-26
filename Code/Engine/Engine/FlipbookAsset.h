#pragma once

#include <Core/Array.h>

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
	};

	struct FlipbookAsset : public eng::Asset
	{
		Array<FlipbookFrame> m_Frames = { };
		float m_FPS = 30.f;
		bool m_IsLooping = true;
	};

	class FlipbookAssetLoader final : public eng::AssetLoader
	{
	public:
		bool Save(FlipbookAsset* asset, eng::Visitor& visitor) const { return true; }
		bool Load(FlipbookAsset* asset, eng::Visitor& visitor) const;
	};
}