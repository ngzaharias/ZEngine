#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorSettingsComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

namespace
{
	const str::StringView strOpen = "m_Open";
	const str::StringView strSave = "m_Save";
}

template<>
void Visitor::ReadCustom(editor::entity::SettingsComponent& value) const
{
	Read(strOpen, value.m_Open, value.m_Open);
	Read(strSave, value.m_Save, value.m_Save);
}
template<>
void Visitor::WriteCustom(const editor::entity::SettingsComponent& value)
{
	Write(strOpen, value.m_Open);
	Write(strSave, value.m_Save);
}