#include "GameUIPCH.h"
#include "GameUI/SettingsMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsComponents.h"
#include "GameClient/SettingsComponents.h"
#include "GameUI/SettingsComponents.h"
#include "Hidden/HiddenSettingsComponents.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

namespace
{
	constexpr Vector2f s_DefaultSize = Vector2f(600.f, 0.f);
}

void gui::settings::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	const bool hasWindow = world.HasAny<ecs::query::Include<gui::settings::WindowComponent>>();
	if (!hasWindow && world.HasAny<ecs::query::Include<gui::settings::OpenRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Settings Menu");

		auto& window = world.AddComponent<gui::settings::WindowComponent>(windowEntity);
		window.m_DebugClient = world.ReadSingleton<clt::settings::DebugComponent>();
		window.m_DebugEngine = world.ReadSingleton<eng::settings::DebugComponent>();
		window.m_DebugHidden = world.ReadSingleton<hidden::settings::DebugComponent>();
		window.m_Local = world.ReadSingleton<eng::settings::LocalComponent>();

		ImGui::OpenPopup("Settings");
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
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoDocking;

		auto& window = world.WriteComponent<gui::settings::WindowComponent>(entity);

		const Vector2f viewportSize = ImGui::GetWindowViewport()->Size;
		const Vector2f viewportCentre = (viewportSize * 0.5f);

		imgui::SetNextWindowPos(viewportCentre, Vector2f(0.5f));
		imgui::SetNextWindowSize(s_DefaultSize);

		bool isWindowOpen = true;
		if (ImGui::BeginPopupModal("Settings", &isWindowOpen, s_WindowFlags))
		{
			if (ImGui::BeginTabBar("##tabs"))
			{
				if (ImGui::BeginTabItem("Gameplay"))
				{
					imgui::Inspector inspector;
					if (inspector.Begin("##settingsmenu"))
					{
						window.m_Local = world.ReadSingleton<eng::settings::LocalComponent>();
						if (inspector.Write(window.m_Local))
							world.WriteSingleton<eng::settings::LocalComponent>() = window.m_Local;
						inspector.End();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Graphics"))
				{
					imgui::Inspector inspector;
					if (inspector.Begin("##settingsmenu"))
					{
						window.m_Graphics = world.ReadSingleton<eng::settings::GraphicsComponent>();
						if (inspector.Write(window.m_Graphics))
							world.WriteSingleton<eng::settings::GraphicsComponent>() = window.m_Graphics;
						inspector.End();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Debug"))
				{
					imgui::Inspector inspector;
					if (inspector.Begin("##settingsmenu"))
					{
						if (inspector.Write(window.m_DebugClient))
							world.WriteSingleton<clt::settings::DebugComponent>() = window.m_DebugClient;
						ImGui::Spacing();
						if (inspector.Write(window.m_DebugEngine))
							world.WriteSingleton<eng::settings::DebugComponent>() = window.m_DebugEngine;
						ImGui::Spacing();
						if (inspector.Write(window.m_DebugHidden))
							world.WriteSingleton<hidden::settings::DebugComponent>() = window.m_DebugHidden;
						inspector.End();
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndPopup();
		}

		if (!isWindowOpen)
			world.DestroyEntity(entity);
	}
}
