#include "EnginePCH.h"
#include "Engine/InspectorHelpers.h"

#include "Core/Guid.h"
#include "Engine/AssetManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

namespace
{
	const str::Name strFlipbook = NAME("Flipbook");
	const str::Name strFont = NAME("Font");
	const str::Name strSprite = NAME("Sprite");
	const str::Name strStaticMesh = NAME("StaticMesh");
	const str::Name strTexture2D = NAME("Texture2D");

	bool Contains(const eng::AssetFile* file, const str::StringView& substring)
	{
		if (!file)
			return false;
		if (substring.empty())
			return true;
		if (str::Contains_NoCase(file->m_Name, substring))
			return true;
		return false;
	}

	str::String ToName(const eng::AssetFile* file, const str::Guid& guid)
	{
		return std::format("{}", file ? file->m_Name.ToChar() : guid.ToString());
	}

	bool WriteAsset(const eng::AssetManager& manager, const str::Name& type, const char* label, str::Guid& value)
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

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("eng::AssetFile"))
				{
					const eng::AssetFile& file = *(const eng::AssetFile*)payload->Data;
					value = file.m_Guid;
					result = true;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SetNextItemWidth(-1);
			if (ImGui::BeginCombo("##combo", preview.c_str(), s_ComboFlags))
			{
				if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
					ImGui::SetKeyboardFocusHere(0);
				imgui::InputText("##search", search);

				const auto& typeMap = manager.GetTypeMap();
				for (const str::Guid& guid : typeMap.Get(type))
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
}

bool imgui::WriteFlipbook(const eng::AssetManager& manager, const char* label, str::Guid& value)
{
	return WriteAsset(manager, strFlipbook, label, value);
}

bool imgui::WriteFont(const eng::AssetManager& manager, const char* label, str::Guid& value)
{
	return WriteAsset(manager, strFont, label, value);
}

bool imgui::WriteSprite(const eng::AssetManager& manager, const char* label, str::Guid& value)
{
	return WriteAsset(manager, strSprite, label, value);
}

bool imgui::WriteStaticMesh(const eng::AssetManager& manager, const char* label, str::Guid& value)
{
	return WriteAsset(manager, strStaticMesh, label, value);
}

bool imgui::WriteTexture2D(const eng::AssetManager& manager, const char* label, str::Guid& value)
{
	return WriteAsset(manager, strTexture2D, label, value);
}
