#include "GameUIPCH.h"
#include "GameUI/SettingsMenu.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsComponents.h"
#include "GameUI/SettingsComponents.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace
{
	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiTableFlags s_InspectorFlags = 0;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const ecs::Entity& entity)
	{
		return std::format("{}: {}", label, entity.GetIndex());
	}
}

void gui::settings::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	const bool hasWindow = world.HasAny<ecs::query::Include<gui::settings::WindowComponent>>();
	if (!hasWindow && world.HasAny<ecs::query::Include<gui::settings::OpenRequestComponent>>())
	{
		const auto& localComponent = world.ReadSingleton< eng::settings::LocalComponent>();

		const ecs::Entity entity = world.CreateEntity();
		auto& windowComponent = world.AddComponent<gui::settings::WindowComponent>(entity);
		windowComponent.m_Label = ToLabel("Settings Menu", entity);
		windowComponent.m_EffectVolume = localComponent.m_EffectVolume;
		windowComponent.m_MusicVolume = localComponent.m_MusicVolume;
	}
	
	if (hasWindow && world.HasAny<ecs::query::Include<gui::settings::CloseRequestComponent>>())
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::settings::CloseRequestComponent>>())
			world.RemoveComponent<gui::settings::WindowComponent>(entity);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<gui::settings::WindowComponent>>())
	{
		auto& windowComponent = world.WriteComponent<gui::settings::WindowComponent>(entity);

		bool isWindowOpen = true;
		if (ImGui::Begin(windowComponent.m_Label.c_str(), &isWindowOpen, s_WindowFlags))
		{
			ImGui::SliderInt("Effect Volume", &windowComponent.m_EffectVolume, 0, 100);
			ImGui::SliderInt("Music Volume", &windowComponent.m_MusicVolume, 0, 100);

			if (ImGui::Button("Apply"))
			{
				auto& localComponent = world.WriteSingleton<eng::settings::LocalComponent>();
				localComponent.m_EffectVolume = windowComponent.m_EffectVolume;
				localComponent.m_MusicVolume = windowComponent.m_MusicVolume;
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

#undef SETVALUE