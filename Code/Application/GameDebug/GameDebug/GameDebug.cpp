#include "GameDebugPCH.h"
#include "GameDebug/GameDebug.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/TypeRegistry.h"
#include "ECS/WorldView.h"
#include "GameDebug/ContainerWindowComponent.h"
#include "GameDebug/ContainerSystem.h"
#include "GameDebug/DebugClientWindowEvent.h"
#include "GameDebug/DebugContainerWindowEvent.h"
#include "GameDebug/DebugEntityWindowEvent.h"
#include "GameDebug/DebugFrameBufferWindowEvent.h"
#include "GameDebug/DebugImGuiDemoEvent.h"
#include "GameDebug/DebugImNodesDemoEvent.h"
#include "GameDebug/DebugInspectorDemoEvent.h"
#include "GameDebug/DebugLevelOpenEvent.h"
#include "GameDebug/DebugLevelReloadEvent.h"
#include "GameDebug/DebugLevelSaveAsEvent.h"
#include "GameDebug/DebugLevelSaveEvent.h"
#include "GameDebug/DebugNetworkWindowEvent.h"
#include "GameDebug/DebugServerWindowEvent.h"
#include "GameDebug/DebugShapeWindowEvent.h"
#include "GameDebug/DebugSplineWindowEvent.h"
#include "GameDebug/EditorAssetBrowserWindowEvent.h"
#include "GameDebug/EditorSettingsWindowEvent.h"
#include "GameDebug/EntitySystem.h"
#include "GameDebug/EntityWindowComponent.h"
#include "GameDebug/FrameBufferSystem.h"
#include "GameDebug/FrameBufferWindowComponent.h"
#include "GameDebug/HiddenObjectComponent.h"
#include "GameDebug/HiddenObjectSystem.h"
#include "GameDebug/ImGuiDemoSystem.h"
#include "GameDebug/ImGuiDemoWindowComponent.h"
#include "GameDebug/ImNodesDemoSystem.h"
#include "GameDebug/ImNodesDemoWindowComponent.h"
#include "GameDebug/InputSystem.h"
#include "GameDebug/InspectorDemoSystem.h"
#include "GameDebug/InspectorDemoWindowComponent.h"
#include "GameDebug/LevelOpenSystem.h"
#include "GameDebug/LevelOpenWindowComponent.h"
#include "GameDebug/LevelReloadSystem.h"
#include "GameDebug/MenuBarSystem.h"
#include "GameDebug/NetworkEvent.h"
#include "GameDebug/NetworkSystem.h"
#include "GameDebug/NetworkWindowComponent.h"
#include "GameDebug/OverlaySystem.h"
#include "GameDebug/PhysicsSystem.h"
#include "GameDebug/SettingsWindowComponent.h"
#include "GameDebug/SettingsWindowEvent.h"
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
	m_ClientWorld.RegisterComponent<debug::ContainerWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::EntityWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::FrameBufferWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::hidden::ObjectComponent>();
	m_ClientWorld.RegisterComponent<debug::ImGuiDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::ImNodesDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::InspectorDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::level::OpenWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::NetworkWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::ShapeWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::SplineWindowComponent>();
	m_ClientWorld.RegisterComponent<debug::settings::WindowComponent>();

	m_ClientWorld.RegisterEvent<debug::ClientWindowEvent>();
	m_ClientWorld.RegisterEvent<debug::ContainerWindowEvent>();
	m_ClientWorld.RegisterEvent<debug::EntityWindowEvent>();
	m_ClientWorld.RegisterEvent<debug::FrameBufferWindowEvent>();
	m_ClientWorld.RegisterEvent<debug::ImGuiDemoEvent>();
	m_ClientWorld.RegisterEvent<debug::ImNodesDemoEvent>();
	m_ClientWorld.RegisterEvent<debug::InspectorDemoEvent>();
	m_ClientWorld.RegisterEvent<debug::level::OpenEvent>();
	m_ClientWorld.RegisterEvent<debug::level::ReloadEvent>();
	m_ClientWorld.RegisterEvent<debug::level::SaveAsEvent>();
	m_ClientWorld.RegisterEvent<debug::level::SaveEvent>();
	m_ClientWorld.RegisterEvent<debug::NetworkEvent>();
	m_ServerWorld.RegisterEvent<debug::NetworkEvent>();
	m_ClientWorld.RegisterEvent<debug::NetworkWindowEvent>();
	m_ClientWorld.RegisterEvent<debug::ServerWindowEvent>();
	m_ClientWorld.RegisterEvent<debug::settings::WindowEvent>();
	m_ClientWorld.RegisterEvent<debug::ShapeWindowEvent>();
	m_ClientWorld.RegisterEvent<debug::SplineWindowEvent>();
	m_ClientWorld.RegisterEvent<editor::AssetBrowserWindowEvent>();
	m_ClientWorld.RegisterEvent<editor::settings::WindowEvent>();

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
	m_ClientWorld.RegisterSystem<debug::NetworkSystem>(m_ClientWorld, m_ServerWorld);
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
	PROFILE_FUNCTION();
}

void debug::GameDebug::Shutdown()
{
	PROFILE_FUNCTION();
}

void debug::GameDebug::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();
}