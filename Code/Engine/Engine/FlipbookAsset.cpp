#include "EnginePCH.h"
#include "Engine/FlipbookAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/TomlHelpers.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strFps = "m_Fps";
	const str::StringView strFrames = "m_Frames";
	const str::StringView strIsLooping = "m_IsLooping";
	const str::StringView strPosition = "m_Position";
	const str::StringView strShader = "m_Shader";
	const str::StringView strSize = "m_Size";
	const str::StringView strTexture2D = "m_Texture2D";

	const eng::FlipbookAsset s_DefaultAsset = { };
	const eng::FlipbookFrame s_DefaultFrame = { };
}

template<>
void eng::Visitor::ReadCustom(eng::FlipbookFrame& value) const
{
	Read(strPosition, value.m_Position, s_DefaultFrame.m_Position);
	Read(strSize, value.m_Size, s_DefaultFrame.m_Size);
}

bool eng::FlipbookAssetLoader::Save(FlipbookAsset& asset, eng::Visitor& visitor) const
{
	visitor.Read(strFps, asset.m_FPS, s_DefaultAsset.m_FPS);
	visitor.Read(strFrames, asset.m_Frames, s_DefaultAsset.m_Frames);
	visitor.Read(strIsLooping, asset.m_IsLooping, s_DefaultAsset.m_IsLooping);
	visitor.Read(strShader, asset.m_Shader, s_DefaultAsset.m_Shader);
	visitor.Read(strTexture2D, asset.m_Texture2D, s_DefaultAsset.m_Texture2D);
	return true;
}

bool eng::FlipbookAssetLoader::Load(FlipbookAsset& asset, eng::Visitor& visitor) const
{
	visitor.Read(strFps, asset.m_FPS, s_DefaultAsset.m_FPS);
	visitor.Read(strFrames, asset.m_Frames, s_DefaultAsset.m_Frames);
	visitor.Read(strShader, asset.m_Shader, s_DefaultAsset.m_Shader);
	visitor.Read(strTexture2D, asset.m_Texture2D, s_DefaultAsset.m_Texture2D);
	return true;
}