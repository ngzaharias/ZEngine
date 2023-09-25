#include "EnginePCH.h"
#include "Engine/FlipbookAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/TomlHelpers.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strFps = NAME("m_Fps");
	const str::Name strFrames = NAME("m_Frames");
	const str::Name strIsLooping = NAME("m_IsLooping");
	const str::Name strPosition = NAME("m_Position");
	const str::Name strShader = NAME("m_Shader");
	const str::Name strSize = NAME("m_Size");
	const str::Name strStaticMesh = NAME("m_StaticMesh");
	const str::Name strTexture2D = NAME("m_Texture2D");

	const eng::FlipbookAsset s_DefaultAsset = { };
	const eng::FlipbookFrame s_DefaultFrame = { };
}

template<>
void eng::Visitor::VisitCustom<eng::FlipbookFrame>(eng::FlipbookFrame& value)
{
	SetInline();
	Visit(strPosition, value.m_Position, s_DefaultFrame.m_Position);
	Visit(strSize, value.m_Size, s_DefaultFrame.m_Size);
}

bool eng::FlipbookAssetLoader::Save(FlipbookAsset* asset, eng::Visitor& visitor) const
{
	visitor.Visit(strFps, asset->m_FPS, s_DefaultAsset.m_FPS);
	visitor.Visit(strFrames, asset->m_Frames, s_DefaultAsset.m_Frames);
	visitor.Visit(strIsLooping, asset->m_IsLooping, s_DefaultAsset.m_IsLooping);
	visitor.Visit(strShader, asset->m_Shader, s_DefaultAsset.m_Shader);
	visitor.Visit(strStaticMesh, asset->m_StaticMesh, s_DefaultAsset.m_StaticMesh);
	visitor.Visit(strTexture2D, asset->m_Texture2D, s_DefaultAsset.m_Texture2D);
	return true;
}

bool eng::FlipbookAssetLoader::Load(FlipbookAsset* asset, eng::Visitor& visitor) const
{
	visitor.Visit(strFps, asset->m_FPS, s_DefaultAsset.m_FPS);
	visitor.Visit(strFrames, asset->m_Frames, s_DefaultAsset.m_Frames);
	visitor.Visit(strShader, asset->m_Shader, s_DefaultAsset.m_Shader);
	visitor.Visit(strStaticMesh, asset->m_StaticMesh, s_DefaultAsset.m_StaticMesh);
	visitor.Visit(strTexture2D, asset->m_Texture2D, s_DefaultAsset.m_Texture2D);
	return true;
}