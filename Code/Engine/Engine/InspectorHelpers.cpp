#include "EnginePCH.h"
#include "Engine/InspectorHelpers.h"

#include "Core/Guid.h"
#include "Engine/AssetManager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_user.h>
#include <imgui/Inspector.h>

namespace
{
	bool Contains(const eng::AssetFile* file, const str::StringView& substring)
	{
		if (!file)
			return false;

		if (substring.empty())
			return true;

		if (str::Contains_NoCase(file->m_Name, substring))
			return true;

		if (str::Contains_NoCase(file->m_Guid.ToString(), substring))
			return true;

		return false;
	}

	str::String ToName(const eng::AssetFile* file, const str::Guid& guid)
	{
		return std::format("{}", file ? file->m_Name.ToChar() : guid.ToString());
	}
}

bool imgui::WriteAsset(const char* label, str::Guid& value, const eng::AssetManager& manager)
{
	static str::String search = {};
	constexpr ImGuiComboFlags s_ComboFlags =
		ImGuiComboFlags_HeightLarge;

	ImGui::TableNextRow();

	bool result = false;
	imgui::RaiiID id(label);
	ImGui::TableSetColumnIndex(0);
	ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);
	{
		imgui::RaiiIndent indent(0);
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-1);

		const eng::AssetFile* parent = manager.GetAssetFile(value);
		const str::String preview = ToName(parent, value);
		imgui::InputText("##value", value);

		ImGui::SetNextItemWidth(-1);
		if (ImGui::BeginCombo("##combo", preview.c_str(), s_ComboFlags))
		{
			imgui::InputText("##search", search);

			static const str::Name s_Sprite = str::Name::Create("Sprite");
			for (const str::Guid& guid : manager.m_TypeMap.Get(s_Sprite))
			{
				const eng::AssetFile* child = manager.GetAssetFile(guid);
				if (!Contains(child, search))
					continue;

				imgui::RaiiID id(&guid);
				bool isSelected = guid == value;
				const str::String label = ToName(child, guid);
				if (imgui::Selectable(label, isSelected))
				{
					result = true;
					value = guid;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					imgui::Text(child->m_Path);
					imgui::Text(child->m_Guid);
					ImGui::EndTooltip();
				}
			}
			ImGui::EndCombo();
		}

		if (parent && ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			imgui::Text(parent->m_Path);
			ImGui::EndTooltip();
		}
	}
	return result;
}
