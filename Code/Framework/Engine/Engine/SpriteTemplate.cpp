#include "EnginePCH.h"
#include "Engine/SpriteTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strColour = "m_Colour";
	const str::StringView strSize = "m_Size";
	const str::StringView strSprite = "m_Sprite";
}

template<>
void Visitor::ReadCustom(eng::SpriteTemplate& value) const
{
	Read(strSprite, value.m_Sprite, value.m_Sprite);
	Read(strColour, value.m_Colour, value.m_Colour);
	Read(strSize, value.m_Size, value.m_Size);
}
template<>
void Visitor::WriteCustom(const eng::SpriteTemplate& value)
{
	Write(strSprite, value.m_Sprite);
	Write(strColour, value.m_Colour);
	Write(strSize, value.m_Size);
}