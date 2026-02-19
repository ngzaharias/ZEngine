#include "ThemeEditorPCH.h"
#include "ThemeEditor/ThemeEditorWindowSystem.h"

#include "Core/EnumHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameState/GameStateEditorComponent.h"
#include "ThemeEditor/ThemeEditorWindowComponent.h"
#include "ThemeEditor/ThemeEditorWindowRequest.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"

namespace
{
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	void DrawColour(ImVec4* colors, const ImGuiCol_ index)
	{
		constexpr ImGuiColorEditFlags_ flags = ImGuiColorEditFlags_NoInputs;
		const str::StringView label = EnumToString<ImGuiCol_>(index);
		ImGui::ColorEdit4(label.data(), &colors[index].x, flags);
	}
}

void editor::theme::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasAny<ecs::query::Include<gamestate::EditorComponent>>())
		return;

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const auto& request : world.Events<editor::theme::WindowRequest>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Theme Editor");

		auto& window = world.AddComponent<editor::theme::WindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Theme Editor", identifier);
	}

	for (auto&& view : world.Query<ecs::query::Removed<const editor::theme::WindowComponent>>())
	{
		const auto& window = world.ReadComponent<editor::theme::WindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& view : world.Query<ecs::query::Include<editor::theme::WindowComponent>>())
	{
		auto& window = view.WriteRequired<editor::theme::WindowComponent>();

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isOpen, s_WindowFlags))
		{
			ImVec4* colors = ImGui::GetStyle().Colors;
			DrawColour(colors, ImGuiCol_Text);
			DrawColour(colors, ImGuiCol_TextDisabled);

			DrawColour(colors, ImGuiCol_WindowBg);
			DrawColour(colors, ImGuiCol_ChildBg);

			DrawColour(colors, ImGuiCol_PopupBg);

			DrawColour(colors, ImGuiCol_Border);
			DrawColour(colors, ImGuiCol_BorderShadow);

			DrawColour(colors, ImGuiCol_FrameBg);
			DrawColour(colors, ImGuiCol_FrameBgHovered);
			DrawColour(colors, ImGuiCol_FrameBgActive);

			DrawColour(colors, ImGuiCol_TitleBg);
			DrawColour(colors, ImGuiCol_TitleBgActive);
			DrawColour(colors, ImGuiCol_TitleBgCollapsed);

			DrawColour(colors, ImGuiCol_Tab);
			DrawColour(colors, ImGuiCol_TabHovered);
			//colors[ImGuiCol_TabActive]              = ImVec4(0.30f, 0.70f, 0.52f, 1.00f);
			//colors[ImGuiCol_TabUnfocused]           = ImVec4(0.10f, 0.14f, 0.12f, 0.97f);
			//colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.18f, 0.35f, 0.26f, 1.00f);

			DrawColour(colors, ImGuiCol_MenuBarBg);
			DrawColour(colors, ImGuiCol_ScrollbarBg);
			DrawColour(colors, ImGuiCol_ScrollbarGrab);
			DrawColour(colors, ImGuiCol_ScrollbarGrabHovered);
			DrawColour(colors, ImGuiCol_ScrollbarGrabActive);

			DrawColour(colors, ImGuiCol_CheckMark);
			DrawColour(colors, ImGuiCol_SliderGrab);
			DrawColour(colors, ImGuiCol_SliderGrabActive);

			DrawColour(colors, ImGuiCol_Button);
			DrawColour(colors, ImGuiCol_ButtonHovered);
			DrawColour(colors, ImGuiCol_ButtonActive);

			DrawColour(colors, ImGuiCol_Header);
			DrawColour(colors, ImGuiCol_HeaderHovered);
			DrawColour(colors, ImGuiCol_HeaderActive);

			DrawColour(colors, ImGuiCol_Separator);
			DrawColour(colors, ImGuiCol_SeparatorHovered);
			DrawColour(colors, ImGuiCol_SeparatorActive);

			DrawColour(colors, ImGuiCol_ResizeGrip);
			DrawColour(colors, ImGuiCol_ResizeGripHovered);
			DrawColour(colors, ImGuiCol_ResizeGripActive);

			DrawColour(colors, ImGuiCol_DockingPreview);
			DrawColour(colors, ImGuiCol_DockingEmptyBg);

			DrawColour(colors, ImGuiCol_PlotLines);
			DrawColour(colors, ImGuiCol_PlotLinesHovered);
			DrawColour(colors, ImGuiCol_PlotHistogram);
			DrawColour(colors, ImGuiCol_PlotHistogramHovered);

			DrawColour(colors, ImGuiCol_TableHeaderBg);
			DrawColour(colors, ImGuiCol_TableBorderStrong);
			DrawColour(colors, ImGuiCol_TableBorderLight);
			DrawColour(colors, ImGuiCol_TableRowBg);
			DrawColour(colors, ImGuiCol_TableRowBgAlt);

			DrawColour(colors, ImGuiCol_TextSelectedBg);
			DrawColour(colors, ImGuiCol_DragDropTarget);
			//colors[ImGuiCol_NavHighlight]           = ImVec4(0.52f, 0.80f, 0.09f, 1.00f);
			DrawColour(colors, ImGuiCol_NavWindowingHighlight);
			DrawColour(colors, ImGuiCol_NavWindowingDimBg);
			DrawColour(colors, ImGuiCol_ModalWindowDimBg);
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(view);
	}
}