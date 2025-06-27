#include "GameUIPCH.h"
#include "GameUI/SettingsMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsComponents.h"
#include "Engine/WindowManager.h"
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
		world.AddComponent<gui::settings::WindowComponent>(windowEntity);

		ImGui::OpenPopup("Settings Menu##gameui");
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
		if (ImGui::BeginPopupModal("Settings Menu##gameui", &isWindowOpen, s_WindowFlags))
		{
			if (ImGui::BeginTabBar("##tabs"))
			{
				imgui::Inspector inspector;
				inspector.AddPayload(world.ReadResource<eng::WindowManager>());

				auto copyLocal = world.ReadSingleton<eng::settings::LocalComponent>();
				if (ImGui::BeginTabItem("Audio"))
				{
					if (inspector.Begin("##settingsmenu"))
					{
						if (inspector.Write(copyLocal.m_Audio))
						{
							auto& writeLocal = world.WriteSingleton<eng::settings::LocalComponent>();
							writeLocal.m_Audio = copyLocal.m_Audio;
						}
						inspector.End();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Gameplay"))
				{
					if (inspector.Begin("##settingsmenu"))
					{
						if (inspector.Write("Camera", copyLocal.m_Camera))
						{
							auto& writeLocal = world.WriteSingleton<eng::settings::LocalComponent>();
							writeLocal.m_Camera = copyLocal.m_Camera;
						}
						inspector.End();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Graphics"))
				{
					if (inspector.Begin("##settingsmenu"))
					{
						if (inspector.Write(copyLocal.m_Window))
						{
							auto& writeLocal = world.WriteSingleton<eng::settings::LocalComponent>();
							writeLocal.m_Window = copyLocal.m_Window;
						}
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
