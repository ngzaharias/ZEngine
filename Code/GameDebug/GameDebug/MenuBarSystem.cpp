#include "GameDebugPCH.h"
#include "GameDebug/MenuBarSystem.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <imgui/imgui.h>

#include "GameDebug/MenuBarComponents.h"

void dbg::MenuBarSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Level", "Ctrl+O"))
				world.AddEventComponent<dbg::level::OpenRequestComponent>();
			if (ImGui::MenuItem("Save Level", "Ctrl+S", nullptr, false))
				world.AddEventComponent<dbg::level::SaveRequestComponent>();
			if (ImGui::MenuItem("Save Level As...", "Ctrl+S+A", nullptr, false))
				world.AddEventComponent<dbg::level::SaveAsRequestComponent>();

			ImGui::EndMenu();
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("Editors"))
		{
			if (ImGui::MenuItem("Flipbook Editor"))
				world.AddEventComponent<editor::FlipbookWindowRequestComponent>();
			if (ImGui::MenuItem("Sprite Editor"))
				world.AddEventComponent<editor::SpriteWindowRequestComponent>();
			if (ImGui::MenuItem("Table Editor"))
				world.AddEventComponent<editor::TableWindowRequestComponent>();
			if (ImGui::MenuItem("Texture Editor"))
				world.AddEventComponent<editor::TextureWindowRequestComponent>();
			if (ImGui::MenuItem("Trajectory Editor"))
				world.AddEventComponent<editor::TrajectoryWindowRequestComponent>();
			if (ImGui::MenuItem("Transform Editor"))
				world.AddEventComponent<editor::TransformWindowRequestComponent>();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Debug: Entities", "Ctrl+Shift+F11"))
				world.AddEventComponent<dbg::EntityWindowRequestComponent>();
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