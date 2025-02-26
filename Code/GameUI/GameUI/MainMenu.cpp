#include "GameUIPCH.h"
#include "GameUI/MainMenu.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/LevelComponents.h"
#include "GameUI/MainMenuComponents.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace
{
	constexpr Vector2f s_DefaultSize = Vector2f(200.f, -1.f);
}

void gui::main_menu::MainMenu::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::main_menu::WindowComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar;

		auto& window = world.WriteComponent<gui::main_menu::WindowComponent>(entity);

		const Vector2f viewportPos = ImGui::GetWindowViewport()->Pos;
		const Vector2f viewportSize = ImGui::GetWindowViewport()->Size;
		const Vector2f viewportCentre = viewportPos + (viewportSize * 0.5f);

		ImGui::SetNextWindowPos(viewportCentre - (s_DefaultSize * 0.5f));
		ImGui::SetNextWindowSize(s_DefaultSize);
		if (ImGui::Begin(window.m_Label.c_str(), nullptr, s_WindowFlags))
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
				if (ImGui::Selectable("Exit Game"))
					world.AddEventComponent<eng::application::CloseRequestComponent>();

				ImGui::EndTable();
			}
			ImGui::PopStyleVar();
		}

		ImGui::End();
	}
}