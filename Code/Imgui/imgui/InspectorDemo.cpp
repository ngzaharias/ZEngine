#include "imgui/InspectorDemo.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

template<>
void imgui::Inspector::ReadCustom(const StructSimple& value)
{
	Read("m_Float", value.m_Float);
	Read("m_Int32", value.m_Int32);
}

template<>
bool imgui::Inspector::WriteCustom(StructSimple& value)
{
	bool result = false;
	result |= Write("m_Float", value.m_Float);
	result |= Write("m_Int32", value.m_Int32);
	return result;
}

template<>
void imgui::Inspector::ReadCustom(const StructRecursive& value)
{
	Read("m_Struct", value.m_Struct);
}

template<>
bool imgui::Inspector::WriteCustom(StructRecursive& value)
{
	bool result = false;
	result |= Write("m_Struct", value.m_Struct);
	return result;
}

void imgui::InspectorDemo(bool& isWindowOpen)
{
	if (!ImGui::Begin("Inspector Demo", &isWindowOpen))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	static bool isReading = false;
	ImGui::Checkbox("IsReadOnly", &isReading);

	if (ImGui::BeginTabBar("##read/write"))
	{
		imgui::InspectorDemo_Basic(isReading);

		imgui::InspectorDemo_Struct(isReading);
		imgui::InspectorDemo_Optional(isReading);
		imgui::InspectorDemo_Variant(isReading);

		imgui::InspectorDemo_Array(isReading);
		imgui::InspectorDemo_Map(isReading);
		imgui::InspectorDemo_Set(isReading);

		ImGui::EndTabBar();
	}

	ImGui::End();
}
