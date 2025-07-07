#include "GameUIPCH.h"
#include "GameUI/MainMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/LevelComponents.h"
#include "GameUI/MainMenuComponents.h"
#include "GameUI/SettingsComponents.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"

namespace
{
	constexpr Vector2f s_DefaultSize = Vector2f(200.f, -1.f);
	constexpr Vector2f s_OffsetPos = Vector2f(0.f, -100.f);
}

void gui::main_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	return;

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::main_menu::WindowComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar;

		auto& window = world.WriteComponent<gui::main_menu::WindowComponent>(entity);

		const Vector2f viewportSize = ImGui::GetWindowViewport()->Size;
		const Vector2f viewportCentre = (viewportSize * 0.5f);

		imgui::SetNextWindowPos(viewportCentre, Vector2f(0.5f));
		imgui::SetNextWindowSize(s_DefaultSize);
		if (ImGui::Begin("Main Menu", nullptr, s_WindowFlags))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginTable("##table", 1))
			{
				ImGui::TableNextRow();
				ImGui::PushItemWidth(-1);

				ImGui::TableNextColumn();
				if (ImGui::Selectable("New Game"))
					world.AddEventComponent<eng::level::LoadRequestComponent>(window.m_NewGame);

				ImGui::TableNextColumn();
				if (ImGui::Selectable("Settings"))
					world.AddEventComponent<gui::settings::OpenRequestComponent>();

				ImGui::TableNextColumn();
				if (ImGui::Selectable("Exit Game"))
					world.AddEventComponent<eng::application::CloseRequestComponent>();

				ImGui::EndTable();
			}
			ImGui::PopStyleVar();
		}

		ImGui::End();
	}
}