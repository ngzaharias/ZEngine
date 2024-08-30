#include "EnginePCH.h"
#include "Engine/SpriteAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strPosition = "m_Position";
	const str::StringView strShader = "m_Shader";
	const str::StringView strSize = "m_Size";
	const str::StringView strTexture2D = "m_Texture2D";

	// defaults
	const str::Guid uuidShader = GUID("cbbb7d3f-f44b-45fd-b9e5-a207d92262fb");
	const str::Guid uuidTexture2D = GUID("c6bb231c-e97f-104e-860e-b55e71988bdb");
}

bool eng::SpriteAssetLoader::Save(SpriteAsset& asset, eng::Visitor& visitor) const
{
	visitor.Write(strPosition, asset.m_Position);
	visitor.Write(strShader, asset.m_Shader);
	visitor.Write(strSize, asset.m_Size);
	visitor.Write(strTexture2D, asset.m_Texture2D);
	return true;
}

bool eng::SpriteAssetLoader::Load(SpriteAsset& asset, eng::Visitor& visitor) const
{
	visitor.Read(strPosition, asset.m_Position, Vector2u::Zero);
	visitor.Read(strShader, asset.m_Shader, uuidShader);
	visitor.Read(strSize, asset.m_Size, Vector2u(1024));
	visitor.Read(strTexture2D, asset.m_Texture2D, uuidTexture2D);
	return true;
}