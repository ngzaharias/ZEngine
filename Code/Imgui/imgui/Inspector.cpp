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
		ImGui::TableNextRow();
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
	imgui::Checkbox("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const float& value)
{
	imgui::DragFloat("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const double& value)
{
	imgui::DragDouble("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const int8& value)
{
	imgui::DragInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const int16& value)
{
	imgui::DragInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const int32& value)
{
	imgui::DragInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const int64& value)
{
	imgui::DragInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const uint8& value)
{
	imgui::DragUInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const uint16& value)
{
	imgui::DragUInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const uint32& value)
{
	imgui::DragUInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const uint64& value)
{
	imgui::DragUInt("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Quaternion& value)
{
	imgui::DragQuaternion("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Rotator& value)
{
	imgui::DragRotator("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector2f& value)
{
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector2i& value)
{
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector2u& value)
{
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector3f& value)
{
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector3i& value)
{
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector4f& value)
{
	imgui::DragVector("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const str::Guid& value)
{
	imgui::Guid("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const str::Name& value)
{
	imgui::Name("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const str::Path& value)
{
	imgui::Path("##value", value);
}

template<>
void imgui::Inspector::ReadCustom(const str::String& value)
{
	imgui::String("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(bool& value)
{
	return imgui::Checkbox("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(float& value)
{
	return imgui::DragFloat("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(double& value)
{
	return imgui::DragDouble("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(int8& value)
{
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(int16& value)
{
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(int32& value)
{
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(int64& value)
{
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(uint8& value)
{
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(uint16& value)
{
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(uint32& value)
{
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(uint64& value)
{
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Quaternion& value)
{
	return imgui::DragQuaternion("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Rotator& value)
{
	return imgui::DragRotator("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector2f& value)
{
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector2i& value)
{
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector2u& value)
{
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector3f& value)
{
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector3i& value)
{
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector4f& value)
{
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(str::Guid& value)
{
	return imgui::Guid("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(str::Name& value)
{
	return imgui::Name("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(str::Path& value)
{
	return imgui::Path("##value", value);
}

template<>
bool imgui::Inspector::WriteCustom(str::String& value)
{
	return imgui::String("##value", value);
}