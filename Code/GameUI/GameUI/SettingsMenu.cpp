#include "GameUIPCH.h"
#include "GameUI/SettingsMenu.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsComponents.h"
#include "GameClient/SettingsComponents.h"
#include "GameUI/SettingsComponents.h"

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

void gui::settings::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	const bool hasWindow = world.HasAny<ecs::query::Include<gui::settings::WindowComponent>>();
	if (!hasWindow && world.HasAny<ecs::query::Include<gui::settings::OpenRequestComponent>>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Settings Menu");

		auto& window = world.AddComponent<gui::settings::WindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Settings Menu", identifier);
		window.m_DebugClient = world.ReadSingleton<clt::settings::DebugComponent>();
		window.m_DebugEngine = world.ReadSingleton<eng::settings::DebugComponent>();
		window.m_Local = world.ReadSingleton<eng::settings::LocalComponent>();
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const gui::settings::WindowComponent>>())
	{
		auto& window = world.ReadComponent<gui::settings::WindowComponent>(entity, false);
		m_WindowIds.Release(window.m_Identifier);
	}
	
	if (hasWindow && world.HasAny<ecs::query::Include<gui::settings::CloseRequestComponent>>())
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::settings::CloseRequestComponent>>())
			world.RemoveComponent<gui::settings::WindowComponent>(entity);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::settings::WindowComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_MenuBar;

		auto& windowComponent = world.WriteComponent<gui::settings::WindowComponent>(entity);

		bool isWindowOpen = true;
		if (ImGui::Begin(windowComponent.m_Label.c_str(), &isWindowOpen, s_WindowFlags))
		{
			if (ImGui::BeginTabBar("##tabs"))
			{
				if (ImGui::BeginTabItem("Local"))
				{
					imgui::Inspector inspector;
					if (inspector.Begin("##settingsmenu"))
					{
						if (inspector.Write(windowComponent.m_Local))
						{
							world.WriteSingleton<eng::settings::LocalComponent>() = windowComponent.m_Local;
						}
						inspector.End();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Debug"))
				{
					imgui::Inspector inspector;
					if (inspector.Begin("##settingsmenu"))
					{
						if (inspector.Write(windowComponent.m_DebugClient))
							world.WriteSingleton<clt::settings::DebugComponent>() = windowComponent.m_DebugClient;
						ImGui::Spacing();
						if (inspector.Write(windowComponent.m_DebugEngine))
							world.WriteSingleton<eng::settings::DebugComponent>() = windowComponent.m_DebugEngine;
						inspector.End();
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();

		if (!isWindowOpen)
			world.DestroyEntity(entity);
	}
}
