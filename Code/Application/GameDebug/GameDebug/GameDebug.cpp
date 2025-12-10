#include "GameDebugPCH.h"
#include "GameDebug/GameDebug.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/ContainerSystem.h"
#include "GameDebug/DebugBufferWindowRequest.h"
#include "GameDebug/DebugClientWindowRequest.h"
#include "GameDebug/DebugContainerWindowRequest.h"
#include "GameDebug/DebugEntityWindowRequest.h"
#include "GameDebug/DebugImGuiDemoRequest.h"
#include "GameDebug/DebugImNodesDemoRequest.h"
#include "GameDebug/DebugInspectorDemoRequest.h"
#include "GameDebug/DebugLevelOpenRequest.h"
#include "GameDebug/DebugLevelReloadRequest.h"
#include "GameDebug/DebugLevelSaveAsRequest.h"
#include "GameDebug/DebugLevelSaveRequest.h"
#include "GameDebug/DebugNetworkWindowRequest.h"
#include "GameDebug/DebugServerWindowRequest.h"
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
#include "GameDebug/EntitySystem.h"
#include "GameDebug/FrameBufferSystem.h"
#include "GameDebug/HiddenObjectComponent.h"
#include "GameDebug/HiddenObjectSystem.h"
#include "GameDebug/ImGuiDemoSystem.h"
#include "GameDebug/ImNodesDemoSystem.h"
#include "GameDebug/InputSystem.h"
#include "GameDebug/InspectorDemoSystem.h"
#include "GameDebug/LevelOpenSystem.h"
#include "GameDebug/LevelOpenWindowComponent.h"
#include "GameDebug/LevelReloadSystem.h"
#include "GameDebug/MenuBarSystem.h"
#include "GameDebug/NetworkWindowComponent.h"
#include "GameDebug/NetworkSystem.h"
#include "GameDebug/OverlaySystem.h"
#include "GameDebug/PhysicsSystem.h"
#include "GameDebug/SettingsWindowComponent.h"
#include "GameDebug/SettingsWindowRequest.h"
#include "GameDebug/SettingsDebugSystem.h"
#include "GameDebug/SettingsMenuSystem.h"
#include "GameDebug/ShapeWindowComponent.h"
#include "GameDebug/ShapeSystem.h"
#include "GameDebug/SoftbodySystem.h"
#include "GameDebug/SplineWindowComponent.h"
#include "GameDebug/SplineSystem.h"

dbg::GameDebug::GameDebug(
	ecs::EntityWorld& clientWorld,
	ecs::EntityWorld& serverWorld)
	: m_ClientWorld(clientWorld)
	, m_ServerWorld(serverWorld)
{
}

void dbg::GameDebug::Register()
{
	m_ClientWorld.RegisterComponent<dbg::BufferWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::ContainerWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::EntityWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::hidden::ObjectComponent>();
	m_ClientWorld.RegisterComponent<dbg::ImGuiDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::ImNodesDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::InspectorDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::level::OpenWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::NetworkWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::ShapeWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::SplineWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::settings::WindowComponent>();

	m_ClientWorld.RegisterEvent<dbg::BufferWindowRequest>();
	m_ClientWorld.RegisterEvent<dbg::ClientWindowRequest>();
	m_ClientWorld.RegisterEvent<dbg::ContainerWindowRequest>();
	m_ClientWorld.RegisterEvent<dbg::EntityWindowRequest>();
	m_ClientWorld.RegisterEvent<dbg::ImGuiDemoRequest>();
	m_ClientWorld.RegisterEvent<dbg::ImNodesDemoRequest>();
	m_ClientWorld.RegisterEvent<dbg::InspectorDemoRequest>();
	m_ClientWorld.RegisterEvent<dbg::level::OpenRequest>();
	m_ClientWorld.RegisterEvent<dbg::level::ReloadRequest>();
	m_ClientWorld.RegisterEvent<dbg::level::SaveAsRequest>();
	m_ClientWorld.RegisterEvent<dbg::level::SaveRequest>();
	m_ClientWorld.RegisterEvent<dbg::NetworkWindowRequest>();
	m_ClientWorld.RegisterEvent<dbg::ServerWindowRequest>();
	m_ClientWorld.RegisterEvent<dbg::settings::WindowRequest>();
	m_ClientWorld.RegisterEvent<dbg::ShapeWindowRequest>();
	m_ClientWorld.RegisterEvent<dbg::SplineWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::AssetBrowserWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::EntityWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::FlipbookWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::InputWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::settings::WindowRequest>();
	m_ClientWorld.RegisterEvent<editor::SpriteWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::TableWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::TextureWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::TrajectoryWindowRequest>();

	m_ClientWorld.RegisterSystem<dbg::ContainerSystem>();
	m_ClientWorld.RegisterSystem<dbg::EntitySystem>(m_ClientWorld, m_ServerWorld);
	m_ClientWorld.RegisterSystem<dbg::FrameBufferSystem>();
	m_ClientWorld.RegisterSystem<dbg::hidden::ObjectSystem>();
	m_ClientWorld.RegisterSystem<dbg::ImGuiDemoSystem>();
	m_ClientWorld.RegisterSystem<dbg::ImNodesDemoSystem>();
	m_ClientWorld.RegisterSystem<dbg::InputSystem>();
	m_ClientWorld.RegisterSystem<dbg::InspectorDemoSystem>();
	m_ClientWorld.RegisterSystem<dbg::level::OpenSystem>();
	m_ClientWorld.RegisterSystem<dbg::level::ReloadSystem>();
	m_ClientWorld.RegisterSystem<dbg::MenuBarSystem>();
	m_ClientWorld.RegisterSystem<dbg::NetworkSystem>();
	m_ClientWorld.RegisterSystem<dbg::OverlaySystem>();
	m_ClientWorld.RegisterSystem<dbg::PhysicsSystem>();
	m_ClientWorld.RegisterSystem<dbg::settings::DebugSystem>();
	m_ClientWorld.RegisterSystem<dbg::settings::MenuSystem>();
	m_ClientWorld.RegisterSystem<dbg::ShapeSystem>();
	m_ClientWorld.RegisterSystem<dbg::SoftbodySystem>();
	m_ClientWorld.RegisterSystem<dbg::SplineSystem>();
}

void dbg::GameDebug::Initialise()
{
}

void dbg::GameDebug::Shutdown()
{
}

void dbg::GameDebug::Update(const GameTime& gameTime)
{
}