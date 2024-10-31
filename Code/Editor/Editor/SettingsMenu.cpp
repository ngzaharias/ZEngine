#include "EditorPCH.h"
#include "Editor/SettingsMenu.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsComponents.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/Inspector.h>

namespace
{
	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}
}

void editor::settings::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const editor::settings::WindowRequestComponent>>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Settings Menu");

		auto& window = world.AddComponent<editor::settings::WindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Settings Menu##editor", identifier);
		window.m_Local = world.ReadSingleton<editor::settings::LocalComponent>();
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const editor::settings::WindowComponent>>())
	{
		auto& window = world.ReadComponent<editor::settings::WindowComponent>(entity, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<editor::settings::WindowComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_MenuBar;

		auto& windowComponent = world.WriteComponent<editor::settings::WindowComponent>(entity);

		bool isWindowOpen = true;
		if (ImGui::Begin(windowComponent.m_Label.c_str(), &isWindowOpen, s_WindowFlags))
		{
			auto& localSettings = windowComponent.m_Local;
			auto& gizmoSettings = localSettings.m_Gizmos;

			{
				imgui::Inspector inspector;
				if (inspector.Begin("##local"))
				{
					inspector.Write("Gizmos", gizmoSettings);
					inspector.End();
				}
			}

			if (ImGui::Button("Apply"))
			{
				world.WriteSingleton<editor::settings::LocalComponent>() = windowComponent.m_Local;
			}
			ImGui::SameLine();
			if (ImGui::Button("Close"))
				isWindowOpen = false;
		}
		ImGui::End();

		if (!isWindowOpen)
			world.DestroyEntity(entity);
	}
}
