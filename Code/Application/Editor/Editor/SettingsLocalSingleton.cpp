#include "EditorPCH.h"
#include "Editor/SettingsLocalSingleton.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strEntity = "m_Entity";
	const str::StringView strGizmos = "m_Gizmos";
	const str::StringView strOpen = "m_Open";
	const str::StringView strSave = "m_Save";
}

template<>
void eng::Visitor::ReadCustom(editor::settings::LocalSingleton& value) const
{
	Read(strEntity, value.m_Entity, value.m_Entity);
	Read(strGizmos, value.m_Gizmos, value.m_Gizmos);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::LocalSingleton& value)
{
	Write(strEntity, value.m_Entity);
	Write(strGizmos, value.m_Gizmos);
}

template<>
void eng::Visitor::ReadCustom(editor::settings::Entity& value) const
{
	Read(strOpen, value.m_Open, value.m_Open);
	Read(strSave, value.m_Save, value.m_Save);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Entity& value)
{
	Write(strOpen, value.m_Open);
	Write(strSave, value.m_Save);
}
