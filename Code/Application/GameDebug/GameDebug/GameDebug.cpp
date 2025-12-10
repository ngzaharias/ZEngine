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

debug::GameDebug::GameDebug(
	ecs::EntityWorld& clientWorld,
	ecs::EntityWorld& serverWorld)
	: m_ClientWorld(clientWorld)
	, m_ServerWorld(serverWorld)
{
}

void debug::GameDebug::Register()
{
	m_ClientWorld.RegisterComponent<debug::BufferWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::ContainerWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::EntityWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::hidden::ObjectComponent>();
	m_ClientWorld.RegisterComponent<debug::ImGuiDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::ImNodesDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::InspectorDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::level::OpenWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::NetworkWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::ShapeWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::SplineWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::settings::WindowComponent>();

	m_ClientWorld.RegisterEvent<debug::BufferWindowRequest>();
	m_ClientWorld.RegisterEvent<debug::ClientWindowRequest>();
	m_ClientWorld.RegisterEvent<debug::ContainerWindowRequest>();
	m_ClientWorld.RegisterEvent<debug::EntityWindowRequest>();
	m_ClientWorld.RegisterEvent<debug::ImGuiDemoRequest>();
	m_ClientWorld.RegisterEvent<debug::ImNodesDemoRequest>();
	m_ClientWorld.RegisterEvent<debug::InspectorDemoRequest>();
	m_ClientWorld.RegisterEvent<debug::level::OpenRequest>();
	m_ClientWorld.RegisterEvent<debug::level::ReloadRequest>();
	m_ClientWorld.RegisterEvent<debug::level::SaveAsRequest>();
	m_ClientWorld.RegisterEvent<debug::level::SaveRequest>();
	m_ClientWorld.RegisterEvent<debug::NetworkWindowRequest>();
	m_ClientWorld.RegisterEvent<debug::ServerWindowRequest>();
	m_ClientWorld.RegisterEvent<debug::settings::WindowRequest>();
	m_ClientWorld.RegisterEvent<debug::ShapeWindowRequest>();
	m_ClientWorld.RegisterEvent<debug::SplineWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::AssetBrowserWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::EntityWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::FlipbookWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::InputWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::settings::WindowRequest>();
	m_ClientWorld.RegisterEvent<editor::SpriteWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::TableWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::TextureWindowRequest>();
	m_ClientWorld.RegisterEvent<editor::TrajectoryWindowRequest>();

	m_ClientWorld.RegisterSystem<debug::ContainerSystem>();
	m_ClientWorld.RegisterSystem<debug::EntitySystem>(m_ClientWorld, m_ServerWorld);
	m_ClientWorld.RegisterSystem<debug::FrameBufferSystem>();
	m_ClientWorld.RegisterSystem<debug::hidden::ObjectSystem>();
	m_ClientWorld.RegisterSystem<debug::ImGuiDemoSystem>();
	m_ClientWorld.RegisterSystem<debug::ImNodesDemoSystem>();
	m_ClientWorld.RegisterSystem<debug::InputSystem>();
	m_ClientWorld.RegisterSystem<debug::InspectorDemoSystem>();
	m_ClientWorld.RegisterSystem<debug::level::OpenSystem>();
	m_ClientWorld.RegisterSystem<debug::level::ReloadSystem>();
	m_ClientWorld.RegisterSystem<debug::MenuBarSystem>();
	m_ClientWorld.RegisterSystem<debug::NetworkSystem>();
	m_ClientWorld.RegisterSystem<debug::OverlaySystem>();
	m_ClientWorld.RegisterSystem<debug::PhysicsSystem>();
	m_ClientWorld.RegisterSystem<debug::settings::DebugSystem>();
	m_ClientWorld.RegisterSystem<debug::settings::MenuSystem>();
	m_ClientWorld.RegisterSystem<debug::ShapeSystem>();
	m_ClientWorld.RegisterSystem<debug::SoftbodySystem>();
	m_ClientWorld.RegisterSystem<debug::SplineSystem>();
}

void debug::GameDebug::Initialise()
{
}

void debug::GameDebug::Shutdown()
{
}

void debug::GameDebug::Update(const GameTime& gameTime)
{
}