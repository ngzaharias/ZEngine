#include "FlipbookEditorPCH.h"
#include "FlipbookEditor/FlipbookEditorSettingsComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

namespace
{
	const str::StringView strExtract = "m_Extract";
	const str::StringView strOpen = "m_Open";
	const str::StringView strSave = "m_Save";
}

template<>
void Visitor::ReadCustom(editor::flipbook::SettingsComponent& value) const
{
	Read(strExtract, value.m_Extract, value.m_Extract);
	Read(strOpen, value.m_Open, value.m_Open);
	Read(strSave, value.m_Save, value.m_Save);
}
template<>
void Visitor::WriteCustom(const editor::flipbook::SettingsComponent& value)
{
	Write(strExtract, value.m_Extract);
	Write(strOpen, value.m_Open);
	Write(strSave, value.m_Save);
}