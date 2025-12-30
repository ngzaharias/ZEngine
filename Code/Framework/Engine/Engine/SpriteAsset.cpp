#include "EnginePCH.h"
#include "Engine/SpriteAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/ShaderAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strPosition = "m_Position";
	const str::StringView strShader = "m_Shader";
	const str::StringView strSize = "m_Size";
	const str::StringView strTexture2D = "m_Texture2D";

	// defaults
	const str::Guid uuidShader = GUID("cbbb7d3ff44b45fdb9e5a207d92262fb");
	const str::Guid uuidTexture2D = GUID("c6bb231ce97f104e860eb55e71988bdb");
}

void eng::SpriteAssetLoader::Bind(eng::SpriteAsset& asset) const
{
	PROFILE_FUNCTION();
	m_AssetManager->RequestAsset(asset.m_Shader);
	m_AssetManager->RequestAsset(asset.m_Texture2D);
}

void eng::SpriteAssetLoader::Unbind(eng::SpriteAsset& asset) const
{
	PROFILE_FUNCTION();
	m_AssetManager->ReleaseAsset(asset.m_Shader);
	m_AssetManager->ReleaseAsset(asset.m_Texture2D);
}

bool eng::SpriteAssetLoader::Load(eng::SpriteAsset& asset, eng::Visitor& visitor) const
{
	PROFILE_FUNCTION();
	visitor.Read(strPosition, asset.m_Position, asset.m_Position);
	visitor.Read(strShader, asset.m_Shader, uuidShader);
	visitor.Read(strSize, asset.m_Size, asset.m_Size);
	visitor.Read(strTexture2D, asset.m_Texture2D, uuidTexture2D);
	return true;
}

bool eng::SpriteAssetLoader::Save(eng::SpriteAsset& asset, eng::Visitor& visitor) const
{
	PROFILE_FUNCTION();
	visitor.Write(strPosition, asset.m_Position);
	visitor.Write(strShader, asset.m_Shader);
	visitor.Write(strSize, asset.m_Size);
	visitor.Write(strTexture2D, asset.m_Texture2D);
	return true;
}