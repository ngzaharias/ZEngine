#include "EditorPCH.h"
#include "Editor/EditorToolbarSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/EditorIcons.h"
#include "Engine/SettingsDebugSingleton.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	using World = editor::ToolbarSystem::World;

	bool ButtonIcon(const char* label, const char* tooltip, const editor::Icon& icon, const ImVec2& size, const bool active = false)
	{
		const bool result = imgui::ImageButton(label, active, icon.m_TextureId, size, icon.m_UV0, icon.m_UV1);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		ImGui::SetItemTooltip(tooltip);
		ImGui::PopStyleVar();
		return result;
	}

	void VerticalSeparator()
	{
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(10.0f, 0.0f));
		ImGui::SameLine();
	}

	void Draw_Mode(World& world)
	{
		const auto& settings = world.ReadSingleton<eng::settings::DebugSingleton>();
		const char* tooltip = settings.m_IsEditorModeEnabled
			? "Edit Mode"
			: "Game Mode";
		const editor::Icon& icon = settings.m_IsEditorModeEnabled
			? icon::MODE_EDIT
			: icon::MODE_GAME;

		if (ButtonIcon("##mode", tooltip, icon, ImVec2(22, 22)))
		{
			auto& write = world.WriteSingleton<eng::settings::DebugSingleton>();
			write.m_IsEditorModeEnabled = !write.m_IsEditorModeEnabled;
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
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(4, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));
	if (ImGui::Begin("Toolbar", nullptr, s_WindowFlags)) 
	{
		ImGui::Dummy(ImVec2(14.0f, 0.0f));
		ImGui::SameLine();

		Draw_Mode(world);
		VerticalSeparator();
	}
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(4);
}