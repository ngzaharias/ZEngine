#include "EnginePCH.h"
#include "Engine/TemplateComponent.h"

#include "imgui/Inspector.h"

namespace
{
	const str::StringView strGuid = "m_Guid";
	const str::StringView strPath = "m_Path";
}

template<>
bool imgui::Inspector::WriteCustom(eng::TemplateComponent& value)
{
	bool result = false;
	result |= Write("m_Guid", value.m_Guid);
	result |= Write("m_Path", value.m_Path);
	return result;
}