#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorSettingsSingleton.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strOpen = "m_Open";
	const str::StringView strSave = "m_Save";
}

template<>
void eng::Visitor::ReadCustom(editor::entity::SettingsSingleton& value) const
{
	Read(strOpen, value.m_Open, value.m_Open);
	Read(strSave, value.m_Save, value.m_Save);
}
template<>
void eng::Visitor::WriteCustom(const editor::entity::SettingsSingleton& value)
{
	Write(strOpen, value.m_Open);
	Write(strSave, value.m_Save);
}