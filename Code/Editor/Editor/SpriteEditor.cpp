#include "EditorPCH.h"
#include "Editor/SpriteEditor.h"

#include "Editor/InspectorWidgets.h"

#include <Engine/AssetManager.h>
#include <Engine/SpriteAsset.h>
#include <Engine/Texture2DAsset.h>

#include <imgui/imgui.h>

namespace
{
	template<typename Type>
	void Inspect(Type& value)
	{
		imgui::InspectorBegin();
		editor::InspectType(value);
		imgui::InspectorEnd();
	}
}

editor::SpriteEditor::SpriteEditor(asset::Manager& assetManager)
	: m_AssetManager(assetManager)
{
}

editor::SpriteEditor::~SpriteEditor()
{
}

void editor::SpriteEditor::Initialise()
{
}

void editor::SpriteEditor::Destroy()
{
}

void editor::SpriteEditor::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Render();
}

void editor::SpriteEditor::OpenEditor(const str::Guid& guid)
{
	m_Selected = guid;
}

void editor::SpriteEditor::CloseEditor()
{
	m_Selected = { };
	ImGui::CloseCurrentPopup();
}

void editor::SpriteEditor::Render()
{
	constexpr float s_SettingsWidth = 400.f;

	if (!m_Selected)
		return;

	const asset::SpriteAsset& spriteAsset = *m_AssetManager.LoadAsset<asset::SpriteAsset>(*m_Selected);

	const ImVec2 position = { ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f };
	ImGui::SetNextWindowPos(position, ImGuiCond_Once, ImVec2(0.5f, 0.5f));

	bool isOpen = true;
	ImGui::Begin("Sprite Editor", &isOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
					m_AssetManager.SaveAsset<asset::SpriteAsset>(*m_Selected);

				ImGui::MenuItem("Save As...");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text(spriteAsset.m_Path.string().c_str());

		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoScrollbar;
		flags |= ImGuiWindowFlags_NoScrollWithMouse;

		const float s_TextureWidth = ImGui::GetContentRegionAvail().x - s_SettingsWidth;
		const float s_TextureHeight = ImGui::GetContentRegionAvail().y;
		if (ImGui::BeginChild("texture", { s_TextureWidth, s_TextureHeight }, true, flags))
		{
			if (spriteAsset.m_Texture2D.IsValid())
			{
				//const ImVec2 cursorPos = ImGui::GetCursorScreenPos();

				//const asset::Texture2DAsset& Texture2DAsset = *m_AssetManager.LoadAsset<asset::Texture2DAsset>(spriteAsset.m_Texture);
				//ImGui::Image(Texture2DAsset.m_Texture);

				//ImDrawList* draw_list = ImGui::GetWindowDrawList();
				//const ImVec2 min = {
				//	cursorPos.x + spriteAsset.m_RectanglePos.x, 
				//	cursorPos.y + spriteAsset.m_RectanglePos.y };
				//const ImVec2 max = {
				//	min.x + spriteAsset.m_RectangleSize.x, 
				//	min.y + spriteAsset.m_RectangleSize.y };
				//const ImU32 col = ImColor(ImVec4(1.0f, 1.0f, 0.4f, 1.0f));
				//draw_list->AddRect(min, max, col, 0.0f, ImDrawFlags_RoundCornersAll, 1.f);
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("settings", { s_SettingsWidth, 0 }, true))
		{
			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
				Inspect(spriteAsset);
		}
		ImGui::EndChild();
	}
	ImGui::End();

	if (!isOpen)
		CloseEditor();
}