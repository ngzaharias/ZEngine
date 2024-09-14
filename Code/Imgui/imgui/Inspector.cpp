#include "imgui/Inspector.h"

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/Types.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

imgui::Inspector::Inspector()
{
	const ImGuiTableFlags flags =
		ImGuiTableFlags_BordersInnerV |
		ImGuiTableFlags_ContextMenuInBody |
		ImGuiTableFlags_Resizable |
		ImGuiTableFlags_ScrollX |
		ImGuiTableFlags_ScrollY;
	if (ImGui::BeginTable("##table", 2, flags))
	{
		m_IsVisible = true;
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableHeadersRow();
	}
}

imgui::Inspector::~Inspector()
{
	if (m_IsVisible)
		ImGui::EndTable();
}

//////////////////////////////////////////////////////////////////////////
// Read

void imgui::Inspector::Read(const char* label, const bool& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	imgui::Checkbox("##value", value);
}

void imgui::Inspector::Read(const char* label, const float& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::DragFloat("##value", value);
}

void imgui::Inspector::Read(const char* label, const double& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::DragDouble("##value", value);
}

void imgui::Inspector::Read(const char* label, const int8& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

void imgui::Inspector::Read(const char* label, const int16& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

void imgui::Inspector::Read(const char* label, const int32& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

void imgui::Inspector::Read(const char* label, const int64& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

void imgui::Inspector::Read(const char* label, const uint8& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

void imgui::Inspector::Read(const char* label, const uint16& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

void imgui::Inspector::Read(const char* label, const uint32& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

void imgui::Inspector::Read(const char* label, const uint64& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

void imgui::Inspector::Read(const char* label, const str::Guid& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::Guid("##value", value);
}

void imgui::Inspector::Read(const char* label, const str::Name& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::Name("##value", value);
}

void imgui::Inspector::Read(const char* label, const str::Path& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::Path("##value", value);
}

void imgui::Inspector::Read(const char* label, const str::String& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	imgui::String("##value", value);
}

//////////////////////////////////////////////////////////////////////////
// Read - Non-Fundamentals

template<>
void imgui::Inspector::ReadCustom(const Quaternion& value)
{
	imgui::DragQuaternion("##Quaternion", value);
}

template<>
void imgui::Inspector::ReadCustom(const Rotator& value)
{
	imgui::DragRotator("##Rotator", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector2f& value)
{
	imgui::DragVector("##Vector2f", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector2i& value)
{
	imgui::DragVector("##Vector2i", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector2u& value)
{
	imgui::DragVector("##Vector2u", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector3f& value)
{
	imgui::DragVector("##Vector3f", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector3i& value)
{
	imgui::DragVector("##Vector3i", value);
}

template<>
void imgui::Inspector::ReadCustom(const Vector4f& value)
{
	imgui::DragVector("##Vector4f", value);
}

//////////////////////////////////////////////////////////////////////////
// Write

bool imgui::Inspector::Write(const char* label, bool& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	return imgui::Checkbox("##value", value);
}

bool imgui::Inspector::Write(const char* label, float& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::DragFloat("##value", value);
}

bool imgui::Inspector::Write(const char* label, double& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::DragDouble("##value", value);
}

bool imgui::Inspector::Write(const char* label, int8& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

bool imgui::Inspector::Write(const char* label, int16& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

bool imgui::Inspector::Write(const char* label, int32& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

bool imgui::Inspector::Write(const char* label, int64& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

bool imgui::Inspector::Write(const char* label, uint8& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

bool imgui::Inspector::Write(const char* label, uint16& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

bool imgui::Inspector::Write(const char* label, uint32& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

bool imgui::Inspector::Write(const char* label, uint64& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

bool imgui::Inspector::Write(const char* label, str::Guid& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::Guid("##value", value);
}

bool imgui::Inspector::Write(const char* label, str::Name& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::Name("##value", value);
}

bool imgui::Inspector::Write(const char* label, str::Path& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::Path("##value", value);
}

bool imgui::Inspector::Write(const char* label, str::String& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	return imgui::String("##value", value);
}

//////////////////////////////////////////////////////////////////////////
// Write - Non-Fundamentals

template<>
bool imgui::Inspector::WriteCustom(Quaternion& value)
{
	return imgui::DragQuaternion("##Vector2f", value);
}

template<>
bool imgui::Inspector::WriteCustom(Rotator& value)
{
	return imgui::DragRotator("##Vector2f", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector2f& value)
{
	return imgui::DragVector("##Vector2f", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector2i& value)
{
	return imgui::DragVector("##Vector2i", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector2u& value)
{
	return imgui::DragVector("##Vector2u", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector3f& value)
{
	return imgui::DragVector("##Vector3f", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector3i& value)
{
	return imgui::DragVector("##Vector3i", value);
}

template<>
bool imgui::Inspector::WriteCustom(Vector4f& value)
{
	return imgui::DragVector("##Vector4f", value);
}