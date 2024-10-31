#include "imgui/Inspector.h"

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/Types.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

bool imgui::Inspector::Begin(const char* label)
{
	const ImGuiTableFlags flags =
		ImGuiTableFlags_SizingFixedFit |
		ImGuiTableFlags_BordersInnerV |
		ImGuiTableFlags_Resizable |
		ImGuiTableFlags_ScrollY;

	// 1 column for the label
	// 1 column for the data
	// 1 column for extra data
	if (ImGui::BeginTable(label, 3, flags))
	{
		constexpr ImGuiTableColumnFlags s_Column0 = ImGuiTableColumnFlags_WidthFixed;
		constexpr ImGuiTableColumnFlags s_Column1 =
			ImGuiTableColumnFlags_IndentDisable |
			ImGuiTableColumnFlags_WidthStretch;
		constexpr ImGuiTableColumnFlags s_Column2 =
			ImGuiTableColumnFlags_IndentDisable |
			ImGuiTableColumnFlags_NoResize |
			ImGuiTableColumnFlags_WidthFixed;

		ImGui::TableSetupColumn("Name ", s_Column0);
		ImGui::TableSetupColumn("Value", s_Column1);
		ImGui::TableSetupColumn("##Extra", s_Column2);
		ImGui::TableHeadersRow();
		return true;
	}

	return false;
}

void imgui::Inspector::End()
{
	ImGui::EndTable();
}

//////////////////////////////////////////////////////////////////////////

template<>
void imgui::Inspector::ReadCustom(const bool& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::Checkbox("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const float& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragFloat("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const double& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragDouble("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const int8& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const int16& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const int32& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const int64& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const uint8& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const uint16& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const uint32& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const uint64& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Quaternion& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragQuaternion("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Rotator& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragRotator("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector2f& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector2i& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector2u& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector3f& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector3i& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector4f& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const str::Guid& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::InputText("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const str::Name& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::InputText("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const str::Path& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::InputText("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const str::String& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::InputText("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(bool& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::Checkbox("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(float& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragFloat("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(double& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragDouble("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(int8& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(int16& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(int32& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(int64& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(uint8& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(uint16& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(uint32& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(uint64& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Quaternion& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragQuaternion("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Rotator& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragRotator("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector2f& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector2i& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector2u& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector3f& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector3i& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector4f& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(str::Guid& value)
{
	bool result = false;
	ImGui::SetNextItemWidth(-1);
	result |= imgui::InputText("##value", value);
	ImGui::OpenPopupOnItemClick("##guid");

	if (ImGui::BeginPopup("##guid"))
	{
		if (ImGui::Selectable("Generate"))
		{
			result = true;
			value = str::Guid::Generate();
		}
		ImGui::EndPopup();
	}
	return result;
}

template<>
bool imgui::Inspector::WriteCustom(str::Name& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::InputText("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(str::Path& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::InputText("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(str::String& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::InputText("##value", value);
}