#include "GameUIPCH.h"
#include "GameUI/GameMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/InputManager.h"
#include "Engine/LevelComponents.h"
#include "Engine/SettingsComponents.h"
#include "GameUI/GameMenuComponents.h"
#include "GameUI/SettingsComponents.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"

namespace
{
	constexpr Vector2f s_DefaultSize = Vector2f(200.f, -1.f);
	constexpr Vector2f s_OffsetPos = Vector2f(0.f, -100.f);

	const str::Name strInput = str::Name::Create("GameMenu");
	const str::Name strClose = str::Name::Create("GameMenu_Close");
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

		{
			input::Layer layer;
			layer.m_Priority = eng::EInputPriority::GameUI;
			layer.m_Bindings.Emplace(strClose, input::EKey::Escape);

			auto& input = world.WriteResource<eng::InputManager>();
			input.AppendLayer(strInput, layer);
		}
	}

	if (world.HasAny<ecs::query::Removed<gui::game_menu::WindowComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
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

		const Vector2f viewportSize = ImGui::GetWindowViewport()->Size;
		const Vector2f viewportCentre = (viewportSize * 0.5f);

		imgui::SetNextWindowPos(viewportCentre, Vector2f(0.5f));
		imgui::SetNextWindowSize(s_DefaultSize);

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

		const auto& input = world.ReadResource<eng::InputManager>();
		if (!isWindowOpen || input.IsPressed(strClose))
			world.DestroyEntity(entity);
	}
}