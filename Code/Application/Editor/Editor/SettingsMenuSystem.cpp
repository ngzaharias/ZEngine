#include "EditorPCH.h"
#include "Editor/SettingsMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsLocalSingleton.h"
#include "Editor/SettingsWindowComponent.h"
#include "GameDebug/EditorSettingsWindowRequest.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/imgui_internal.h"
#include "imgui/Inspector.h"

namespace
{
	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}
}

void editor::settings::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (const auto& request : world.Events<editor::settings::WindowRequest>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Settings Menu");

		auto& window = world.AddComponent<editor::settings::WindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Settings Menu##editor", identifier);
		window.m_Local = world.ReadSingleton<editor::settings::LocalSingleton>();
	}

	for (auto&& view : world.Query<ecs::query::Removed<const editor::settings::WindowComponent>>())
	{
		auto& window = world.ReadComponent<editor::settings::WindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& view : world.Query<ecs::query::Include<editor::settings::WindowComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_MenuBar;

		auto& windowComponent = view.WriteRequired<editor::settings::WindowComponent>();

		bool isWindowOpen = true;
		if (ImGui::Begin(windowComponent.m_Label.c_str(), &isWindowOpen, s_WindowFlags))
		{
			auto& localSettings = windowComponent.m_Local;
			auto& gizmoSettings = localSettings.m_Gizmos;

			bool wasModified = false;
			{
				imgui::Inspector inspector;
				if (inspector.Begin("##local"))
				{
					wasModified |= inspector.Write("Gizmos", gizmoSettings);
					inspector.End();
				}
			}

			if (wasModified)
			{
				world.WriteSingleton<editor::settings::LocalSingleton>() = localSettings;
			}
		}
		ImGui::End();

		if (!isWindowOpen)
			world.DestroyEntity(view);
	}
}
