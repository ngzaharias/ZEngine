#include "EnginePCH.h"
#include "Engine/FlipbookAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/TomlHelpers.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strFrameCount = NAME("m_FrameCount");
	const str::Name strFrames = NAME("m_Frames");
	const str::Name strFps = NAME("m_Fps");
	const str::Name strSprite = NAME("m_Sprite");

	const eng::FlipbookAsset s_DefaultAsset = { };
	const eng::FlipbookFrame s_DefaultFrame = { };
}

template<>
void eng::Visitor::VisitCustom<eng::FlipbookFrame>(eng::FlipbookFrame& value)
{
	SetInline();
	Visit(strFrameCount, value.m_FrameCount, s_DefaultFrame.m_FrameCount);
	Visit(strSprite, value.m_Sprite, s_DefaultFrame.m_Sprite);
}

bool eng::FlipbookAssetLoader::Load(FlipbookAsset* asset, eng::Visitor& visitor) const
{
	visitor.Visit(strFps, asset->m_FPS, s_DefaultAsset.m_FPS);
	visitor.Visit(strFrames, asset->m_Frames, s_DefaultAsset.m_Frames);
	return true;
}