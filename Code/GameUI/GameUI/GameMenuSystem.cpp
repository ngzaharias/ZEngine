#include "GameUIPCH.h"
#include "GameUI/GameMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/LevelComponents.h"
#include "GameUI/GameMenuComponents.h"
#include "GameUI/SettingsComponents.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace
{
	constexpr Vector2f s_DefaultSize = Vector2f(200.f, -1.f);
	constexpr Vector2f s_OffsetPos = Vector2f(0.f, -100.f);
}

void gui::game_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	const auto& settings = world.ReadSingleton<eng::settings::LaunchComponent>();

	const bool hasWindow = world.HasAny<ecs::query::Include<gui::game_menu::WindowComponent>>();
	if (!hasWindow && world.HasAny<ecs::query::Include<gui::game_menu::OpenRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Game Menu");
		world.AddComponent<gui::game_menu::WindowComponent>(windowEntity);

		ImGui::OpenPopup("Game Menu");
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::game_menu::WindowComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoDocking;

		auto& window = world.WriteComponent<gui::game_menu::WindowComponent>(entity);

		const Vector2f viewportPos = ImGui::GetWindowViewport()->Pos;
		const Vector2f viewportSize = ImGui::GetWindowViewport()->Size;
		const Vector2f viewportCentre = viewportPos + (viewportSize * 0.5f);

		ImGui::SetNextWindowPos(viewportCentre - (s_DefaultSize * 0.5f) + s_OffsetPos);
		ImGui::SetNextWindowSize(s_DefaultSize);

		bool isWindowOpen = true;
		if (ImGui::BeginPopupModal("Game Menu", &isWindowOpen, s_WindowFlags))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginTable("##table", 1))
			{
				ImGui::TableNextRow();
				ImGui::PushItemWidth(-1);

				ImGui::TableNextColumn();
				if (ImGui::Selectable("Resume"))
					isWindowOpen = false;

				ImGui::TableNextColumn();
				if (ImGui::Selectable("Settings"))
					world.AddEventComponent<gui::settings::OpenRequestComponent>();

				ImGui::TableNextColumn();
				if (ImGui::Selectable("Exit to Menu"))
					world.AddEventComponent<eng::level::LoadRequestComponent>(settings.m_Level);

				ImGui::TableNextColumn();
				if (ImGui::Selectable("Exit Game"))
					world.AddEventComponent<eng::application::CloseRequestComponent>();

				ImGui::EndTable();
			}
			ImGui::PopStyleVar();

			ImGui::EndPopup();
		}

		if (!isWindowOpen)
			world.DestroyEntity(entity);
	}
}