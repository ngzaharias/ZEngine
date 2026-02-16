#include "EditorPCH.h"
#include "Editor/EditorToolbarSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/EditorIcons.h"
#include "Editor/SettingsLocalSingleton.h"
#include "Engine/SettingsDebugSingleton.h"
#include "EntityEditor/EntityEditorOpenWindowEvent.h"
#include "FlipbookEditor/FlipbookEditorOpenWindowEvent.h"
#include "Settings/SettingsEditorComponent.h"
#include "Settings/SettingsEditorToggleEvent.h"
#include "SpriteEditor/SpriteEditorOpenWindowEvent.h"
#include "TextureEditor/TextureEditorOpenWindowEvent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	using World = editor::ToolbarSystem::World;

	bool ButtonIcon(const char* label, const char* tooltip, const editor::Icon& icon, const ImVec2& size = ImVec2(22, 22), const bool active = false)
	{
		const bool result = imgui::ImageButton(label, active, icon.m_TextureId, size, icon.m_UV0, icon.m_UV1);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		ImGui::SetItemTooltip(tooltip);
		ImGui::PopStyleVar();
		return result;
	}

	void Spacing()
	{
		ImGui::Dummy(ImVec2(10.0f, 0.0f));
		ImGui::SameLine();
	}

	void Draw_Mode(World& world)
	{
		const bool isEnabled = world.HasAny<ecs::query::Include<settings::EditorComponent>>();
		const char* tooltip = isEnabled
			? "Switch to Game Mode"
			: "Switch to Edit Mode";
		const editor::Icon& icon = isEnabled
			? icon::MODE_GAME
			: icon::MODE_EDIT;

		if (ButtonIcon("##mode", tooltip, icon))
		{
			world.AddEvent<settings::EditorToggleEvent>();
		}
		ImGui::SameLine();
	}

	void Draw_Editors(World& world)
	{
		if (ButtonIcon("##entity", "Entity Editor", icon::EDITOR_ENTITY))
			world.AddEvent<editor::entity::OpenWindowEvent>();
		ImGui::SameLine();
		if (ButtonIcon("##texture", "Texture Editor", icon::EDITOR_TEXTURE))
			world.AddEvent<editor::texture::OpenWindowEvent>();
		ImGui::SameLine();
		if (ButtonIcon("##sprite", "Sprite Editor", icon::EDITOR_SPRITE))
			world.AddEvent<editor::sprite::OpenWindowEvent>();
		ImGui::SameLine();
		if (ButtonIcon("##flipbook", "Flipbook Editor", icon::EDITOR_FLIPBOOK))
			world.AddEvent<editor::flipbook::OpenWindowEvent>();
		ImGui::SameLine();
	}

	void Draw_Gizmos(World& world)
	{
		const auto& settings = world.ReadSingleton<editor::settings::LocalSingleton>();
		const auto& axes = settings.m_Gizmos.m_CoordAxes;
		if (ButtonIcon("##axis", "Axes", icon::AXIS, ImVec2(22, 22), axes.m_IsEnabled))
		{
			auto& write = world.WriteSingleton<editor::settings::LocalSingleton>();
			write.m_Gizmos.m_CoordAxes.m_IsEnabled = !axes.m_IsEnabled;
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			ImGui::OpenPopup("axes");
		if (ImGui::BeginPopup("axes"))
		{
			auto& write = world.WriteSingleton<editor::settings::LocalSingleton>();
			ImGui::PushStyleColor(ImGuiCol_CheckMark, axes.m_ColourX);
			ImGui::Checkbox("##X", &write.m_Gizmos.m_CoordAxes.m_ShowX);
			ImGui::PopStyleColor();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_CheckMark, axes.m_ColourY);
			ImGui::Checkbox("##Y", &write.m_Gizmos.m_CoordAxes.m_ShowY);
			ImGui::PopStyleColor();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_CheckMark, axes.m_ColourZ);
			ImGui::Checkbox("##Z", &write.m_Gizmos.m_CoordAxes.m_ShowZ);
			ImGui::PopStyleColor();

			ImGui::EndPopup();
		}
		ImGui::SameLine();

		const auto& grid = settings.m_Gizmos.m_FloorGrid;
		if (ButtonIcon("##grid", "Grid", icon::GRID, ImVec2(22, 22), grid.m_IsEnabled))
		{
			auto& write = world.WriteSingleton<editor::settings::LocalSingleton>();
			write.m_Gizmos.m_FloorGrid.m_IsEnabled = !grid.m_IsEnabled;
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			ImGui::OpenPopup("grid");
		if (ImGui::BeginPopup("grid"))
		{
			auto& write = world.WriteSingleton<editor::settings::LocalSingleton>();
			ImGui::ColorEdit3("Colour  ", &write.m_Gizmos.m_FloorGrid.m_MajorColour.x, ImGuiColorEditFlags_NoInputs);
			ImGui::EndPopup();
		}
		ImGui::SameLine();
	}
}

void editor::ToolbarSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	const float toolbar_height = 50.f;
	const float menu_bar_height = ImGui::GetFrameHeight();

	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menu_bar_height));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, toolbar_height));
	ImGui::SetNextWindowViewport(viewport->ID);

	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoDocking;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 8));
	if (ImGui::Begin("Toolbar", nullptr, s_WindowFlags)) 
	{
		ImGui::PopStyleVar(2);

		Spacing();
		Draw_Mode(world);
		Spacing();
		Draw_Editors(world);
		Spacing();
		Draw_Gizmos(world);
	}
	ImGui::End();
}