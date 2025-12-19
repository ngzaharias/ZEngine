#include "GameDebugPCH.h"
#include "GameDebug/MenuBarSystem.h"

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/InputManager.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/TablesReloaded.h"
#include "GameDebug/DebugContainerWindowRequest.h"
#include "GameDebug/DebugEntityWindowRequest.h"
#include "GameDebug/DebugFrameBufferWindowRequest.h"
#include "GameDebug/DebugImGuiDemoRequest.h"
#include "GameDebug/DebugImNodesDemoRequest.h"
#include "GameDebug/DebugInspectorDemoRequest.h"
#include "GameDebug/DebugLevelOpenRequest.h"
#include "GameDebug/DebugLevelReloadRequest.h"
#include "GameDebug/DebugLevelSaveRequest.h"
#include "GameDebug/DebugNetworkWindowRequest.h"
#include "GameDebug/DebugShapeWindowRequest.h"
#include "GameDebug/DebugSplineWindowRequest.h"
#include "GameDebug/EditorAssetBrowserWindowRequest.h"
#include "GameDebug/EditorEntityWindowRequest.h"
#include "GameDebug/EditorFlipbookWindowRequest.h"
#include "GameDebug/EditorInputWindowRequest.h"
#include "GameDebug/EditorSettingsWindowRequest.h"
#include "GameDebug/EditorSpriteWindowRequest.h"
#include "GameDebug/EditorTableWindowRequest.h"
#include "GameDebug/EditorTextureWindowRequest.h"
#include "GameDebug/EditorTrajectoryWindowRequest.h"
#include "GameDebug/LevelOpenWindowComponent.h"
#include "GameDebug/SettingsWindowComponent.h"
#include "GameDebug/SettingsWindowRequest.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

#include <windows.h>

namespace
{
	const str::Name strInput = str::Name::Create("DebugMenuBar");
	const str::Name strOpen = str::Name::Create("DebugMenuBar_Open");
	const str::Name strReload = str::Name::Create("DebugMenuBar_Reload");
	const str::Name strSave = str::Name::Create("DebugMenuBar_Save");

	void LaunchExe(const str::Path& filepath)
	{
		// #release: programs that run shell commands will be deleted by windows defender
#ifndef Z_RELEASE
		ShellExecuteA(NULL, "open", filepath.ToChar(), NULL, NULL, SW_SHOWDEFAULT);
#endif
	}
}

void debug::MenuBarSystem::Initialise(World& world)
{
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
	auto& input = world.WriteResource<eng::InputManager>();
	input.RemoveLayer(strInput);
}

void debug::MenuBarSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

#ifdef Z_RELEASE
	return;
#endif 

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Level", "Ctrl+O"))
				world.AddEvent<debug::level::OpenRequest>();
			if (ImGui::MenuItem("Reload Level", "Ctrl+R"))
				world.AddEvent<debug::level::ReloadRequest>();
			
			ImGui::Separator();

			if (ImGui::MenuItem("Reload Assets"))
			{
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.ReloadAssets();
			}

			if (ImGui::MenuItem("Reload Tables"))
			{
				world.AddEvent<eng::TablesReloaded>();
				auto& tableHeadmaster = world.WriteResource<eng::TableHeadmaster>();
				tableHeadmaster.Reload();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Debug"))
				world.AddEvent<debug::settings::WindowRequest>();
			if (ImGui::MenuItem("Editor"))
				world.AddEvent<editor::settings::WindowRequest>();
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
			if (ImGui::MenuItem("Container"))
				world.AddEvent<debug::ContainerWindowRequest>();
			if (ImGui::MenuItem("Entities", "Ctrl+Shift+F11"))
				world.AddEvent<debug::EntityWindowRequest>();
			if (ImGui::MenuItem("Network"))
				world.AddEvent<debug::NetworkWindowRequest>();
			if (ImGui::MenuItem("Optick (external)"))
				LaunchExe(str::Path(str::EPath::ThirdParty, "optick/1.3.1/Optick.exe"));
			if (ImGui::MenuItem("Shapes"))
				world.AddEvent<debug::ShapeWindowRequest>();
			if (ImGui::MenuItem("Splines"))
				world.AddEvent<debug::SplineWindowRequest>();
			if (ImGui::MenuItem("Render: Frame Buffer"))
				world.AddEvent<debug::FrameBufferWindowRequest>();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Editors"))
		{
			if (ImGui::MenuItem("Asset Browser"))
				world.AddEvent<editor::AssetBrowserWindowRequest>();
			if (ImGui::MenuItem("Entity Editor"))
				world.AddEvent<editor::EntityWindowRequest>();
			if (ImGui::MenuItem("Flipbook Editor"))
				world.AddEvent<editor::FlipbookWindowRequest>();
			if (ImGui::MenuItem("Input Editor"))
				world.AddEvent<editor::InputWindowRequest>();
			if (ImGui::MenuItem("Sprite Editor"))
				world.AddEvent<editor::SpriteWindowRequest>();
			if (ImGui::MenuItem("Table Editor"))
				world.AddEvent<editor::TableWindowRequest>();
			if (ImGui::MenuItem("Texture Editor"))
				world.AddEvent<editor::TextureWindowRequest>();
			if (ImGui::MenuItem("Trajectory Editor"))
				world.AddEvent<editor::TrajectoryWindowRequest>();
			if (ImGui::MenuItem("UI Editor (external)"))
				LaunchExe(str::Path(str::EPath::Assets, "UI/ZEngine.noesis"));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Demo: ImGui"))
				world.AddEvent<debug::ImGuiDemoRequest>();
			if (ImGui::MenuItem("Demo: ImNodes"))
				world.AddEvent<debug::ImNodesDemoRequest>();
			if (ImGui::MenuItem("Demo: Inspector"))
				world.AddEvent<debug::InspectorDemoRequest>();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	{
		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsPressed(strOpen))
			world.AddEvent<debug::level::OpenRequest>();
		if (input.IsPressed(strReload))
			world.AddEvent<debug::level::ReloadRequest>();
		if (input.IsPressed(strSave))
			world.AddEvent<debug::level::SaveRequest>();
	}
}