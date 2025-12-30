#include "SpriteEditorPCH.h"
#include "SpriteEditor/SpriteEditorSettingsSingleton.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strExtract = "m_Extract";
	const str::StringView strOpen = "m_Open";
	const str::StringView strSave = "m_Save";
}

template<>
void eng::Visitor::ReadCustom(editor::sprite::SettingsSingleton& value) const
{
	Read(strExtract, value.m_Extract, value.m_Extract);
	Read(strOpen, value.m_Open, value.m_Open);
	Read(strSave, value.m_Save, value.m_Save);
}
template<>
void eng::Visitor::WriteCustom(const editor::sprite::SettingsSingleton& value)
{
	Write(strExtract, value.m_Extract);
	Write(strOpen, value.m_Open);
	Write(strSave, value.m_Save);
}