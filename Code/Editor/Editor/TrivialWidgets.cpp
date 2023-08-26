#include "EditorPCH.h"
#include "Editor/TrivialWidgets.h"

#include <Core/Vector.h>

#include <imgui/imgui.h>

void editor::TypeOverload(bool& value)
{
	ImGui::Checkbox("", &value);
}

void editor::TypeOverload(float& value)
{
	ImGui::DragFloat("", &value);
}

void editor::TypeOverload(int32& value)
{
	ImGui::DragInt("", &value);
}

void editor::TypeOverload(uint32& value)
{
	ImGui::DragScalar("", ImGuiDataType_U32, &value, 1.f);
}

void editor::TypeOverload(Quaternion& value)
{
	ImGui::DragFloat4("", &value.x);
}

void editor::TypeOverload(Rotator& value)
{
	ImGui::DragFloat3("", &value.m_Pitch);
	ImGui::DragFloat3("", &value.m_Yaw);
	ImGui::DragFloat3("", &value.m_Roll);
}

void editor::TypeOverload(Vector2f& value)
{
	ImGui::DragFloat2("", &value.x);
}

void editor::TypeOverload(Vector2i& value)
{
	ImGui::DragInt2("", &value.x);
}

void editor::TypeOverload(Vector2u& value)
{
	ImGui::DragScalarN("", ImGuiDataType_U32, &value.x, 2, 1.f);
}

void editor::TypeOverload(Vector3f& value)
{
	ImGui::DragFloat3("", &value.x);
}

void editor::TypeOverload(Vector3i& value)
{
	ImGui::DragInt3("", &value.x);
}

void editor::TypeOverload(str::String& value)
{
	const size_t size = 256;
	char* buffer = new char[size];
	strncpy_s(buffer, size, value.c_str(), size);

	const bool isChanged = ImGui::InputText("", buffer, size);
	value = buffer;
}