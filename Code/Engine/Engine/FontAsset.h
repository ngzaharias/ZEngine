#pragma once

#include <Core/Path.h>
#include <Core/SparseArray.h>

#include <Engine/Asset.h>
#include <Engine/AssetLoader.h>

namespace eng
{
	class Visitor;
}

namespace eng
{
	using Charcode = char;

	struct Glyph
	{
		str::Path m_SourceFile = { };
		uint32 m_AdvanceX = 0;
		uint32 m_AdvanceY = 0;
		float m_Depth = 0.f;
	};

	struct FontAsset : public eng::Asset
	{
		int32 m_Width = 0;
		int32 m_Height = 0;
		int32 m_PixelRange = 0;

		uint32 m_TextureId = 0;
		SparseArray<Charcode, Glyph> m_Glyphs = { };
	};

	class FontAssetLoader final : public eng::AssetLoader
	{
	public:
		bool Save(FontAsset& asset, eng::Visitor& visitor) const;
		bool Load(FontAsset& asset, eng::Visitor& visitor) const;
		bool Import(FontAsset& asset, const str::Path& filepath) const;
	};
}