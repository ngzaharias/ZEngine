#include "EditorPCH.h"
#include "Editor/SettingsLocalComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strGizmos = "m_Gizmos";
}

template<>
void eng::Visitor::ReadCustom(editor::settings::LocalComponent& value) const
{
	Read(strGizmos, value.m_Gizmos, value.m_Gizmos);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::LocalComponent& value)
{
	Write(strGizmos, value.m_Gizmos);
}