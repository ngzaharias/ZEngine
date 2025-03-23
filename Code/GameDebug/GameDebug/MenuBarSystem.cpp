#include "GameDebugPCH.h"
#include "GameDebug/MenuBarSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/InputManager.h"
#include "GameDebug/MenuBarComponents.h"
#include "GameDebug/SettingsComponents.h"
#include "GameUI/SettingsComponents.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/String.h"

// #todo: disable on release builds

namespace
{
	const str::Guid strInputGuid = str::Guid::Generate();
	const str::Name strModifier = str::Name::Create("DebugMenuBar_Modifier");
	const str::Name strOpen = str::Name::Create("DebugMenuBar_Open");
	const str::Name strReload = str::Name::Create("DebugMenuBar_Reload");
	const str::Name strSave = str::Name::Create("DebugMenuBar_Save");
}

void dbg::MenuBarSystem::Initialise(World& world)
{
	input::Layer layer;
	layer.m_Priority = eng::EInputPriority::Debug;
	layer.m_Bindings.Emplace(input::EKeyboard::Control_L, strModifier);
	layer.m_Bindings.Emplace(input::EKeyboard::Control_R, strModifier);
	layer.m_Bindings.Emplace(input::EKeyboard::O, strOpen);
	layer.m_Bindings.Emplace(input::EKeyboard::R, strReload);
	layer.m_Bindings.Emplace(input::EKeyboard::S, strSave);

	auto& input = world.WriteResource<eng::InputManager>();
	input.AppendLayer(strInputGuid, layer);
}

void dbg::MenuBarSystem::Shutdown(World& world)
{
	auto& input = world.WriteResource<eng::InputManager>();
	input.RemoveLayer(strInputGuid);
}

void dbg::MenuBarSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Level", "Ctrl+O"))
				world.AddEventComponent<dbg::level::OpenRequestComponent>();
			if (ImGui::MenuItem("Reload Level", "Ctrl+R"))
				world.AddEventComponent<dbg::level::ReloadRequestComponent>();
			if (ImGui::MenuItem("Save Level", "Ctrl+S", nullptr, false))
				world.AddEventComponent<dbg::level::SaveRequestComponent>();
			if (ImGui::MenuItem("Save Level As...", "Ctrl+S+A", nullptr, false))
				world.AddEventComponent<dbg::level::SaveAsRequestComponent>();
			
			ImGui::Separator();

			if (ImGui::MenuItem("Reload Assets"))
			{
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.ReloadAssets();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Debug"))
				world.AddEventComponent<dbg::settings::WindowRequestComponent>();
			if (ImGui::MenuItem("Editor"))
				world.AddEventComponent<editor::settings::WindowRequestComponent>();
			if (ImGui::MenuItem("Game"))
				world.AddEventComponent<gui::settings::OpenRequestComponent>();
			ImGui::EndMenu();
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("Editors"))
		{
			if (ImGui::MenuItem("Entity Editor"))
				world.AddEventComponent<editor::EntityWindowRequestComponent>();
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

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Debug: Container"))
				world.AddEventComponent<dbg::ContainerWindowRequestComponent>();
			if (ImGui::MenuItem("Debug: Entities", "Ctrl+Shift+F11"))
				world.AddEventComponent<dbg::EntityWindowRequestComponent>();
			if (ImGui::MenuItem("Debug: Shapes"))
				world.AddEventComponent<dbg::ShapeWindowRequestComponent>();
			if (ImGui::MenuItem("Demo: ImGui"))
				world.AddEventComponent<dbg::ImGuiDemoRequestComponent>();
			if (ImGui::MenuItem("Demo: ImNodes"))
				world.AddEventComponent<dbg::ImNodesDemoRequestComponent>();
			if (ImGui::MenuItem("Demo: Inspector"))
				world.AddEventComponent<dbg::InspectorDemoRequestComponent>();
			if (ImGui::MenuItem("Network"))
				world.AddEventComponent<dbg::NetworkWindowRequestComponent>();
			if (ImGui::MenuItem("Render: Frame Buffer"))
				world.AddEventComponent<dbg::BufferWindowRequestComponent>();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	{
		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsHeld(strModifier) && input.IsPressed(strOpen))
			world.AddEventComponent<dbg::level::OpenRequestComponent>();
		if (input.IsHeld(strModifier) && input.IsPressed(strReload))
			world.AddEventComponent<dbg::level::ReloadRequestComponent>();
		if (input.IsHeld(strModifier) && input.IsPressed(strSave))
			world.AddEventComponent<dbg::level::SaveRequestComponent>();
	}
}