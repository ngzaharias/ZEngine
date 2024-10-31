#include "EnginePCH.h"
#include "Engine/FlipbookComponent.h"

#include "Engine/Visitor.h"

namespace 
{
	const str::StringView strFlipbook = "m_Flipbook";
	const str::StringView strIndex = "m_Index";
	const str::StringView strIsLooping = "m_IsLooping";
	const str::StringView strIsPlaying = "m_IsPlaying";
	const str::StringView strSize = "m_Size";
	const str::StringView strTimeStart = "m_TimeStart";
}

template<>
void eng::Visitor::ReadCustom(eng::FlipbookComponent& value) const
{
	Read(strFlipbook, value.m_Flipbook, value.m_Flipbook);
	Read(strSize, value.m_Size, value.m_Size);
	Read(strIndex, value.m_Index, value.m_Index);
	Read(strTimeStart, value.m_TimeStart, value.m_TimeStart);
	Read(strIsLooping, value.m_IsLooping, value.m_IsLooping);
	Read(strIsPlaying, value.m_IsPlaying, value.m_IsPlaying);
}
template<>
void eng::Visitor::WriteCustom(const eng::FlipbookComponent& value)
{
	Write(strFlipbook, value.m_Flipbook);
	Write(strSize, value.m_Size);
	Write(strIndex, value.m_Index);
	Write(strTimeStart, value.m_TimeStart);
	Write(strIsLooping, value.m_IsLooping);
	Write(strIsPlaying, value.m_IsPlaying);
}