#include "EditorPCH.h"
#include "Editor/SpriteExtractor.h"

#include "Editor/InspectorWidgets.h"

#include <Engine/AssetManager.h>
#include <Engine/FileHelpers.h>
#include <Engine/SpriteAsset.h>

#include <functional>

#include <imgui/imgui.h>

namespace
{
	constexpr const char* s_Label = "Sprite Extractor";
	const str::Path strDefaultPath = str::Path("D:\\Projects\\C++\\ZEngine\\Assets\\Sprites");

	template<typename Type>
	void Inspect(Type& value)
	{
		imgui::InspectorBegin();
		editor::InspectType(value);
		imgui::InspectorEnd();
	}

	void ForEachSprite(const editor::GridSettings& settings, std::function<bool(const Vector2u&)> callback)
	{
		Vector2u position = Vector2u::Zero;
		position.y = settings.m_Margin.y;

		for (uint32 y = 0; y < settings.m_Count.y; ++y)
		{
			position.x = settings.m_Margin.x;
			for (uint32 x = 0; x < settings.m_Count.x; ++x)
			{
				if (!callback(position))
					return;
				position.x += settings.m_Size.x;
				position.x += settings.m_Spacing.x;
			}
			position.y += settings.m_Size.y;
			position.y += settings.m_Spacing.y;
		}
	}
}

editor::SpriteExtractor::SpriteExtractor(asset::Manager& assetManager)
	: m_AssetManager(assetManager)
{
}

void editor::SpriteExtractor::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Render();
}

void editor::SpriteExtractor::OpenDialog(const str::Guid& guid)
{
	m_Selected = guid;
}

void editor::SpriteExtractor::CloseDialog()
{
	m_Selected = { };
	ImGui::CloseCurrentPopup();
}

void editor::SpriteExtractor::Render()
{
	constexpr float s_SettingsWidth = 400.f;
	constexpr float s_SettingsBotHeight = 30.f;

	if (!m_Selected)
		return;

	const asset::Texture2DAsset& textureAsset = *m_AssetManager.LoadAsset<asset::Texture2DAsset>(*m_Selected);
	if (!ImGui::IsPopupOpen(s_Label))
	{
		ImGui::OpenPopup(s_Label);
		m_GridSettings.m_Count = { 1, 1 };
		m_GridSettings.m_Size.x = textureAsset.m_Width;
		m_GridSettings.m_Size.y = textureAsset.m_Height;
	}

	const ImVec2 position = { ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f };
	ImGui::SetNextWindowPos(position, ImGuiCond_Once, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(s_Label))
	{
		constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
		if (ImGui::BeginChild("texture", { ImGui::GetContentRegionAvail().x - s_SettingsWidth, 0 }, false, flags))
		{
			ImGui::Text(textureAsset.m_Path.string().c_str());

			const ImVec2 cursorPos = ImGui::GetCursorScreenPos();
			//ImGui::Image(textureAsset.m_Texture);

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ForEachSprite(m_GridSettings, [&](const Vector2u& position)
			{
				const ImVec2 min = { cursorPos.x + position.x, cursorPos.y + position.y };
				const ImVec2 max = { min.x + m_GridSettings.m_Size.x, min.y + m_GridSettings.m_Size.y };
				const ImU32 col = ImColor(ImVec4(1.0f, 1.0f, 0.4f, 1.0f));
				drawList->AddRect(min, max, col, 0.0f, ImDrawFlags_RoundCornersAll, 1.f);
				return true;
			});
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("settings", { ImGui::GetContentRegionAvail().x, 0 }, false))
		{
			const float topWidth = s_SettingsWidth;
			const float topHeight = ImGui::GetWindowHeight() - s_SettingsBotHeight;
			if (ImGui::BeginChild("settings_top", { topWidth, topHeight }, false))
			{
				if (ImGui::CollapsingHeader("Naming", ImGuiTreeNodeFlags_DefaultOpen))
					Inspect(m_NamingSettings);

				if (ImGui::CollapsingHeader("Grid", ImGuiTreeNodeFlags_DefaultOpen))
					Inspect(m_GridSettings);
			}
			ImGui::EndChild();

			const float botWidth = s_SettingsWidth;
			const float botHeight = s_SettingsBotHeight;
			if (ImGui::BeginChild("settings_bottom", { botWidth, botHeight }, false))
			{
				if (ImGui::Button("Extract"))
				{
					if (ExtractDialog())
					{
						CloseDialog();
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
					CloseDialog();
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();

		ImGui::EndPopup();
	}
}

bool editor::SpriteExtractor::ExtractDialog()
{
	core::SelectFolderSettings folderSettings;
	folderSettings.m_Directory = strDefaultPath.string();
	str::Path outputFolder = core::SelectFolderDialog(folderSettings);
	if (outputFolder.empty())
		return false;

	int32 namingIndex = m_NamingSettings.m_StartIndex;

	ForEachSprite(m_GridSettings, [&](const Vector2u& position)
	{
		str::Path outputFilepath = outputFolder;
		outputFilepath += "\\";
		outputFilepath += m_NamingSettings.m_Template;
		outputFilepath += std::to_string(namingIndex++);
		outputFilepath += ".asset";

		asset::SpriteAsset* asset = new asset::SpriteAsset();
		asset->m_Position = position;
		asset->m_Size = m_GridSettings.m_Size;
		asset->m_Texture2D = *m_Selected;

		m_AssetManager.CreateAsset<asset::SpriteAsset>(asset, outputFilepath);
		m_AssetManager.SaveAsset<asset::SpriteAsset>(asset->m_Guid);
		return true;
	});
	return true;
}
