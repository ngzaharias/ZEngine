#include "GameDebugPCH.h"
#include "GameDebug/SettingsMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsDebugSingleton.h"
#include "GameClient/SettingsDebugComponent.h"
#include "GameDebug/SettingsWindowComponent.h"
#include "GameDebug/SettingsWindowRequest.h"
#include "Hidden/HiddenDebugSettingsSingleton.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

namespace
{
	constexpr Vector2f s_DefaultSize = Vector2f(600.f, 0.f);
}

void debug::settings::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const bool hasWindow = world.HasAny<ecs::query::Include<debug::settings::WindowComponent>>();
	if (!hasWindow && world.HasAny<debug::settings::WindowRequest>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Debug Settings");

		auto& window = world.AddComponent<debug::settings::WindowComponent>(windowEntity);
		window.m_Client = world.ReadSingleton<client::settings::DebugSingleton>();
		window.m_Engine = world.ReadSingleton<eng::settings::DebugSingleton>();
		window.m_Hidden = world.ReadSingleton<hidden::settings::DebugSingleton>();

		ImGui::OpenPopup("Debug Settings##gamedebug");
	}

	for (auto&& view : world.Query<ecs::query::Include<debug::settings::WindowComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoCollapse;

		auto& window = view.WriteRequired<debug::settings::WindowComponent>();

		const Vector2f viewportSize = ImGui::GetWindowViewport()->Size;
		const Vector2f viewportCentre = (viewportSize * 0.5f);

		imgui::SetNextWindowPos(viewportCentre, Vector2f(0.5f), ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);

		bool isWindowOpen = true;
		if (ImGui::Begin("Debug Settings##gamedebug", &isWindowOpen, s_WindowFlags))
		{
			imgui::Inspector inspector;
			if (inspector.Begin("##debugsettings"))
			{
				if (inspector.Write(window.m_Client))
					world.WriteSingleton<client::settings::DebugSingleton>() = window.m_Client;
				ImGui::Spacing();
				if (inspector.Write(window.m_Engine))
					world.WriteSingleton<eng::settings::DebugSingleton>() = window.m_Engine;
				ImGui::Spacing();
				if (inspector.Write(window.m_Hidden))
					world.WriteSingleton<hidden::settings::DebugSingleton>() = window.m_Hidden;
				inspector.End();
			}
			ImGui::End();
		}

		if (!isWindowOpen)
			world.DestroyEntity(view);
	}
}
