#include "GameDebugPCH.h"
#include "GameDebug/MenuBarSystem.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <imgui/imgui.h>

#include "GameDebug/MenuBarComponents.h"

void dbg::MenuBarSystem::Update(World& world, const GameTime& gameTime)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Level", "Ctrl+O"))
				world.AddEventComponent<dbg::LevelDialogRequestComponent>();
			if (ImGui::MenuItem("Save Level", "Ctrl+S", nullptr, false))
				world.AddEventComponent<dbg::SaveLevelRequestComponent>();
			if (ImGui::MenuItem("Save Level As...", "Ctrl+S+A", nullptr, false))
				world.AddEventComponent<dbg::SaveLevelAsRequestComponent>();

			ImGui::EndMenu();
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Debug: Entities", "Ctrl+Shift+F11"))
				world.AddEventComponent<dbg::EntityWindowRequestComponent>();
			if (ImGui::MenuItem("Debug: Trajectory", "Ctrl+Shift+F11"))
				world.AddEventComponent<dbg::TrajectoryWindowRequestComponent>();
			if (ImGui::MenuItem("ImGui Demo"))
				world.AddEventComponent<dbg::ImGuiDemoRequestComponent>();
			if (ImGui::MenuItem("Network"))
				world.AddEventComponent<dbg::NetworkWindowRequestComponent>();
			if (ImGui::MenuItem("Node Graph"))
				world.AddEventComponent<dbg::NodeGraphRequestComponent>();
			if (ImGui::MenuItem("Render: Frame Buffer"))
				world.AddEventComponent<dbg::BufferWindowRequestComponent>();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}