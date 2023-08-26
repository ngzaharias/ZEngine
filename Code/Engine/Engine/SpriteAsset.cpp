#include "EnginePCH.h"
#include "Engine/SpriteAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strPosition = NAME("m_Position");
	const str::Name strShader = NAME("m_Shader");
	const str::Name strSize = NAME("m_Size");
	const str::Name strStaticMesh = NAME("m_StaticMesh");
	const str::Name strTexture2D = NAME("m_Texture2D");

	// defaults
	const str::Guid uuidShader = GUID("cbbb7d3f-f44b-45fd-b9e5-a207d92262fb");
	const str::Guid uuidStaticMesh = GUID("e94876a8-e4cc-4d16-84c8-5859b48a1af6");
	const str::Guid uuidTexture2D = GUID("c6bb231c-e97f-104e-860e-b55e71988bdb");
}

bool eng::SpriteAssetLoader::Load(SpriteAsset* asset, eng::Visitor& visitor) const
{
	visitor.Visit(strShader, asset->m_Shader, uuidShader);
	visitor.Visit(strStaticMesh, asset->m_StaticMesh, uuidStaticMesh);
	visitor.Visit(strTexture2D, asset->m_Texture2D, uuidTexture2D);
	visitor.Visit(strPosition, asset->m_Position, Vector2u::Zero);
	visitor.Visit(strSize, asset->m_Size, Vector2u(1024));
	return true;
}