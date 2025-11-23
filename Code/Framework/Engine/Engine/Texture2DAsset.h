#pragma once

#include "Engine/Asset.h"
#include "Engine/AssetLoader.h"

namespace eng
{
	class Visitor;
}

namespace eng
{
	struct Texture2DAsset : public eng::Asset, eng::DeferredLoad
	{
		str::Path m_SourceFile = { };

		int32 m_Channels = 0;
		int32 m_Height = 0;
		int32 m_Width = 0;

		uint32 m_TextureId = 0;

		unsigned char* m_TextureData = nullptr;
	};

	class Texture2DAssetLoader final : public eng::AssetLoader
	{
	public:
		void Initialise(eng::Texture2DAsset& asset) const;
		void Shutdown(eng::Texture2DAsset& asset) const;

		bool Save(eng::Texture2DAsset& asset, eng::Visitor& visitor) const;
		bool Load(eng::Texture2DAsset& asset, eng::Visitor& visitor) const;
		bool Import(eng::Texture2DAsset& asset, const str::Path& filepath) const;
	};
}