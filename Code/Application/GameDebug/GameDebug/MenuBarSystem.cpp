#include "GameDebugPCH.h"
#include "GameDebug/MenuBarSystem.h"

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "DebugCrafting/CraftingWindowEvent.h"
#include "DebugEntity/EntityWindowEvent.h"
#include "DebugInventory/InventoryWindowEvent.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/ExecuteShell.h"
#include "Engine/InputManager.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/TablesReloadedEvent.h"
#include "GameDebug/DebugFrameBufferWindowEvent.h"
#include "GameDebug/DebugImGuiDemoEvent.h"
#include "GameDebug/DebugImNodesDemoEvent.h"
#include "GameDebug/DebugInspectorDemoEvent.h"
#include "GameDebug/DebugLevelOpenEvent.h"
#include "GameDebug/DebugLevelReloadEvent.h"
#include "GameDebug/DebugLevelSaveEvent.h"
#include "GameDebug/DebugNetworkWindowEvent.h"
#include "GameDebug/DebugShapeWindowEvent.h"
#include "GameDebug/DebugSplineWindowEvent.h"
#include "GameDebug/EditorSettingsWindowEvent.h"
#include "GameDebug/LevelOpenWindowComponent.h"
#include "GameDebug/SettingsWindowComponent.h"
#include "GameDebug/SettingsWindowEvent.h"
#include "InputEditor/InputEditorWindowEvent.h"
#include "SpellEditor/SpellEditorWindowEvent.h"
#include "TableEditor/TableEditorWindowEvent.h"
#include "ThemeEditor/ThemeEditorWindowEvent.h"
#include "TrajectoryEditor/TrajectoryEditorWindowEvent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

namespace
{
	const str::Name strInput = str::Name::Create("DebugMenuBar");
	const str::Name strOpen = str::Name::Create("DebugMenuBar_Open");
	const str::Name strReload = str::Name::Create("DebugMenuBar_Reload");
	const str::Name strSave = str::Name::Create("DebugMenuBar_Save");

	void LaunchExe(const str::Path& filepath)
	{
		eng::ExecuteShell("open", filepath.ToChar(), eng::EShellMode::DEFAULT);
	}
}

void debug::MenuBarSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	input::Layer layer;
	layer.m_Priority = eng::EInputPriority::MenuBar;
	layer.m_Bindings.Emplace(strOpen,   input::EKey::O, input::EKey::Control_L);
	layer.m_Bindings.Emplace(strOpen,   input::EKey::O, input::EKey::Control_R);
	layer.m_Bindings.Emplace(strReload, input::EKey::R, input::EKey::Control_L);
	layer.m_Bindings.Emplace(strReload, input::EKey::R, input::EKey::Control_R);

	auto& input = world.WriteResource<eng::InputManager>();
	input.AppendLayer(strInput, layer);
}

void debug::MenuBarSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	auto& input = world.WriteResource<eng::InputManager>();
	input.RemoveLayer(strInput);
}

void debug::MenuBarSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

#if defined(Z_RELEASE) && !defined(Z_EDITOR)
	return;
#endif

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Level", "Ctrl+O"))
				world.AddEvent<debug::level::OpenEvent>();
			if (ImGui::MenuItem("Reload Level", "Ctrl+R"))
				world.AddEvent<debug::level::ReloadEvent>();
			
			ImGui::Separator();

			if (ImGui::MenuItem("Reload Assets"))
			{
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.ReloadAssets();
			}

			if (ImGui::MenuItem("Reload Tables"))
			{
				world.AddEvent<eng::TablesReloadedEvent>();
				auto& tableHeadmaster = world.WriteResource<eng::TableHeadmaster>();
				tableHeadmaster.Reload();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Debug"))
				world.AddEvent<debug::settings::WindowEvent>();
			if (ImGui::MenuItem("Editor"))
				world.AddEvent<editor::settings::WindowEvent>();
			ImGui::EndMenu();
		}

		ImGui::Separator();

		//if (ImGui::BeginMenu("Cheats"))
		//{
		//	if (ImGui::MenuItem("Reveal All"))
		//		world.AddEvent<debug::cheats::RevealAll>();
		//	if (ImGui::MenuItem("Reveal All (-1)"))
		//		world.AddEvent<debug::cheats::RevealAll>(-1);
		//}

		if (ImGui::BeginMenu("Debuggers"))
		{
			if (ImGui::MenuItem("Crafting"))
				world.AddEvent<debug::crafting::WindowEvent>();
			if (ImGui::MenuItem("Entity", "Ctrl+Shift+F11"))
				world.AddEvent<debug::entity::WindowEvent>();
			if (ImGui::MenuItem("Inventory"))
				world.AddEvent<debug::inventory::WindowEvent>();
			if (ImGui::MenuItem("Network"))
				world.AddEvent<debug::NetworkWindowEvent>();
			if (ImGui::MenuItem("Optick (external)"))
				LaunchExe(str::Path(str::EPath::ThirdParty, "optick/1.3.1/Optick.exe"));
			if (ImGui::MenuItem("Shapes"))
				world.AddEvent<debug::ShapeWindowEvent>();
			if (ImGui::MenuItem("Splines"))
				world.AddEvent<debug::SplineWindowEvent>();
			if (ImGui::MenuItem("Render: Frame Buffer"))
				world.AddEvent<debug::FrameBufferWindowEvent>();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Editors"))
		{
			if (ImGui::MenuItem("Input Editor"))
				world.AddEvent<editor::input::WindowEvent>();
			if (ImGui::MenuItem("Spell Editor"))
				world.AddEvent<editor::spell::WindowEvent>();
			if (ImGui::MenuItem("Table Editor"))
				world.AddEvent<editor::table::WindowEvent>();
			if (ImGui::MenuItem("Theme Editor"))
				world.AddEvent<editor::theme::WindowEvent>();
			if (ImGui::MenuItem("Trajectory Editor"))
				world.AddEvent<editor::trajectory::WindowEvent>();
			if (ImGui::MenuItem("UI Editor (external)"))
				LaunchExe(str::Path(str::EPath::Assets, "UI/ZEngine.noesis"));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Demo: ImGui"))
				world.AddEvent<debug::ImGuiDemoEvent>();
			if (ImGui::MenuItem("Demo: ImNodes"))
				world.AddEvent<debug::ImNodesDemoEvent>();
			if (ImGui::MenuItem("Demo: Inspector"))
				world.AddEvent<debug::InspectorDemoEvent>();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	{
		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsPressed(strOpen))
			world.AddEvent<debug::level::OpenEvent>();
		if (input.IsPressed(strReload))
			world.AddEvent<debug::level::ReloadEvent>();
		if (input.IsPressed(strSave))
			world.AddEvent<debug::level::SaveEvent>();
	}
}