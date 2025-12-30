#include "TextureEditorPCH.h"
#include "TextureEditor/TextureEditorSettingsSingleton.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strExtract = "m_Import";
	const str::StringView strOpen = "m_Open";
	const str::StringView strSave = "m_Save";
}

template<>
void eng::Visitor::ReadCustom(editor::texture::SettingsSingleton& value) const
{
	Read(strExtract, value.m_Import, value.m_Import);
	Read(strOpen, value.m_Open, value.m_Open);
	Read(strSave, value.m_Save, value.m_Save);
}
template<>
void eng::Visitor::WriteCustom(const editor::texture::SettingsSingleton& value)
{
	Write(strExtract, value.m_Import);
	Write(strOpen, value.m_Open);
	Write(strSave, value.m_Save);
}