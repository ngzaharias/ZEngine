#include "GameUIPCH.h"
#include "GameUI/SettingsMenu.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
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
		window.m_Label = ToLabel("Settings Menu", identifier);
		window.m_Debug = world.ReadSingleton<eng::settings::DebugComponent>();
		window.m_Local = world.ReadSingleton<eng::settings::LocalComponent>();
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
			if (ImGui::BeginTabBar("##tabs"))
			{
				if (ImGui::BeginTabItem("Local"))
				{
					auto& localSettings = windowComponent.m_Local;
					auto& audioSettings = localSettings.m_Audio;
					auto& cameraSettings = localSettings.m_Camera;

					ImGui::TextDisabled("Audio");
					ImGui::SliderInt("Effect Volume", &audioSettings.m_EffectVolume, 0, 100);
					ImGui::SliderInt("Music Volume", &audioSettings.m_MusicVolume, 0, 100);

					ImGui::TextDisabled("Camera");
					ImGui::DragFloat("Translate Speed", &cameraSettings.m_TranslateSpeed);
					ImGui::DragFloat3("Rotate Speed", &cameraSettings.m_RotateSpeed.m_Pitch);
					ImGui::DragFloat("Zoom Speed", &cameraSettings.m_ZoomSpeed);

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Debug"))
				{
					auto& debugSettings = windowComponent.m_Debug;

					ImGui::Checkbox("Are Gizmos Enabled", &debugSettings.m_AreGizmosEnabled);
					ImGui::Checkbox("Are Hidden Enabled", &debugSettings.m_AreHiddenEnabled);
					ImGui::Checkbox("Are Lines Enabled", &debugSettings.m_AreLinesEnabled);
					ImGui::Checkbox("Are Physics Enabled", &debugSettings.m_ArePhysicsEnabled);

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			if (ImGui::Button("Apply"))
			{
				world.WriteSingleton<eng::settings::DebugComponent>() = windowComponent.m_Debug;
				world.WriteSingleton<eng::settings::LocalComponent>() = windowComponent.m_Local;
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