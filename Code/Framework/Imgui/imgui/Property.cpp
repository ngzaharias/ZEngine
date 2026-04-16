#include "imgui/Property.h"

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/Types.h"
#include "Math/AABB.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

template<>
void imgui::ReadCustom(const bool& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::Checkbox("##value", value);
}

template<>
void imgui::ReadCustom(const float& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragFloat("##value", value);
}

template<>
void imgui::ReadCustom(const double& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragDouble("##value", value);
}

template<>
void imgui::ReadCustom(const int8& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

template<>
void imgui::ReadCustom(const int16& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

template<>
void imgui::ReadCustom(const int32& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

template<>
void imgui::ReadCustom(const int64& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragInt("##value", value);
}

template<>
void imgui::ReadCustom(const uint8& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

template<>
void imgui::ReadCustom(const uint16& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

template<>
void imgui::ReadCustom(const uint32& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

template<>
void imgui::ReadCustom(const uint64& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragUInt("##value", value);
}

template<>
void imgui::ReadCustom(const Colour& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragColour("##value", value);
}

template<>
void imgui::ReadCustom(const Quaternion& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragQuaternion("##value", value);
}

template<>
void imgui::ReadCustom(const Rotator& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragRotator("##value", value);
}

template<>
void imgui::ReadCustom(const Vector2f& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::ReadCustom(const Vector2i& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::ReadCustom(const Vector2u& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::ReadCustom(const Vector3f& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::ReadCustom(const Vector3i& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::ReadCustom(const Vector4f& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::DragVector("##value", value);
}

template<>
void imgui::ReadCustom(const str::Guid& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::InputText("##value", value);
}

template<>
void imgui::ReadCustom(const str::Name& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::InputText("##value", value);
}

template<>
void imgui::ReadCustom(const str::Path& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::InputText("##value", value);
}

template<>
void imgui::ReadCustom(const str::String& value)
{
	ImGui::SetNextItemWidth(-1);
	imgui::InputText("##value", value);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<>
bool imgui::WriteCustom(bool& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::Checkbox("##value", value);
}

template<>
bool imgui::WriteCustom(float& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragFloat("##value", value);
}

template<>
bool imgui::WriteCustom(double& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragDouble("##value", value);
}

template<>
bool imgui::WriteCustom(int8& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::WriteCustom(int16& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::WriteCustom(int32& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::WriteCustom(int64& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragInt("##value", value);
}

template<>
bool imgui::WriteCustom(uint8& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::WriteCustom(uint16& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::WriteCustom(uint32& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::WriteCustom(uint64& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragUInt("##value", value);
}

template<>
bool imgui::WriteCustom(AABB2f& value)
{
	bool modified = false;
	ImGui::SetNextItemWidth(-1);
	modified |= imgui::DragVector("m_Min", value.m_Min);
	ImGui::SetNextItemWidth(-1);
	modified |= imgui::DragVector("m_Max", value.m_Max);
	return modified;
}

template<>
bool imgui::WriteCustom(AABB2i& value)
{
	bool modified = false;
	ImGui::SetNextItemWidth(-1);
	modified |= imgui::DragVector("m_Min", value.m_Min);
	ImGui::SetNextItemWidth(-1);
	modified |= imgui::DragVector("m_Max", value.m_Max);
	return modified;
}

template<>
bool imgui::WriteCustom(AABB3f& value)
{
	bool modified = false;
	ImGui::SetNextItemWidth(-1);
	modified |= imgui::DragVector("m_Min", value.m_Min);
	ImGui::SetNextItemWidth(-1);
	modified |= imgui::DragVector("m_Max", value.m_Max);
	return modified;
}

template<>
bool imgui::WriteCustom(Colour& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragColour("##value", value);
}

template<>
bool imgui::WriteCustom(Quaternion& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragQuaternion("##value", value);
}

template<>
bool imgui::WriteCustom(Rotator& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragRotator("##value", value);
}

template<>
bool imgui::WriteCustom(Vector2f& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::WriteCustom(Vector2i& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::WriteCustom(Vector2u& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::WriteCustom(Vector3f& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::WriteCustom(Vector3i& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::WriteCustom(Vector4f& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::DragVector("##value", value);
}

template<>
bool imgui::WriteCustom(str::Guid& value)
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
bool imgui::WriteCustom(str::Name& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::InputText("##value", value);
}

template<>
bool imgui::WriteCustom(str::Path& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::InputText("##value", value);
}

template<>
bool imgui::WriteCustom(str::String& value)
{
	ImGui::SetNextItemWidth(-1);
	return imgui::InputText("##value", value);
}