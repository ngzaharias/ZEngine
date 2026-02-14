#include "EditorPCH.h"
#include "Editor/SettingsLocalSingleton.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strGizmos = "m_Gizmos";
}

template<>
void eng::Visitor::ReadCustom(editor::settings::LocalSingleton& value) const
{
	Read(strGizmos, value.m_Gizmos, value.m_Gizmos);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::LocalSingleton& value)
{
	Write(strGizmos, value.m_Gizmos);
}