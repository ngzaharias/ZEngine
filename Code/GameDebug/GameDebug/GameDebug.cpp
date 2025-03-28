#include "GameDebugPCH.h"
#include "GameDebug/GameDebug.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/ContainerSystem.h"
#include "GameDebug/EntitySystem.h"
#include "GameDebug/FrameBufferSystem.h"
#include "GameDebug/HiddenObjectComponent.h"
#include "GameDebug/HiddenObjectSystem.h"
#include "GameDebug/ImGuiDemoSystem.h"
#include "GameDebug/ImNodesDemoSystem.h"
#include "GameDebug/InputSystem.h"
#include "GameDebug/InspectorDemoSystem.h"
#include "GameDebug/LevelOpenComponents.h"
#include "GameDebug/LevelOpenSystem.h"
#include "GameDebug/LevelReloadSystem.h"
#include "GameDebug/MenuBarComponents.h"
#include "GameDebug/MenuBarSystem.h"
#include "GameDebug/NetworkComponents.h"
#include "GameDebug/NetworkSystem.h"
#include "GameDebug/OverlaySystem.h"
#include "GameDebug/PhysicsSystem.h"
#include "GameDebug/SettingsComponents.h"
#include "GameDebug/SettingsDebugSystem.h"
#include "GameDebug/SettingsMenuSystem.h"
#include "GameDebug/ShapeComponents.h"
#include "GameDebug/ShapeSystem.h"

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
	m_ClientWorld.RegisterComponent<dbg::BufferWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::ContainerWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::ContainerWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::ClientWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::EntityWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::EntityWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::hidden::ObjectComponent>();
	m_ClientWorld.RegisterComponent<dbg::ImGuiDemoRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::ImGuiDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::ImNodesDemoRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::ImNodesDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::InspectorDemoRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::InspectorDemoWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::level::OpenRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::level::ReloadRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::level::SaveAsRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::level::SaveRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::level::WindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::NetworkWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::NetworkWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::ServerWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::ShapeWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::ShapeWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::settings::WindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::settings::WindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::EntityWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::settings::WindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::TableWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryWindowRequestComponent>();

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