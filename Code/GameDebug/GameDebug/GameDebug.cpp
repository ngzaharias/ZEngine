#include "GameDebugPCH.h"
#include "GameDebug/GameDebug.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "GameDebug/EntitySystem.h"
#include "GameDebug/FrameBufferSystem.h"
#include "GameDebug/ImGuiDemoSystem.h"
#include "GameDebug/MenuBarComponents.h"
#include "GameDebug/MenuBarSystem.h"
#include "GameDebug/NetworkComponents.h"
#include "GameDebug/NetworkSystem.h"
#include "GameDebug/NodeGraphComponents.h"
#include "GameDebug/NodeGraphSystem.h"
#include "GameDebug/OpenLevelSystem.h"
#include "GameDebug/OverlaySystem.h"

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
	m_ClientWorld.RegisterComponent<dbg::ClientWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::EntityWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::EntityWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::ImGuiDemoComponent>();
	m_ClientWorld.RegisterComponent<dbg::ImGuiDemoRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::LevelDialogRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::NetworkWindowComponent>();
	m_ClientWorld.RegisterComponent<dbg::NetworkWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::NodeGraphComponent>();
	m_ClientWorld.RegisterComponent<dbg::NodeGraphRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::SaveLevelAsRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::SaveLevelRequestComponent>();
	m_ClientWorld.RegisterComponent<dbg::ServerWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::TableWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureWindowRequestComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryWindowRequestComponent>();

	m_ClientWorld.RegisterSystem<dbg::EntitySystem>(m_ClientWorld, m_ServerWorld);
	m_ClientWorld.RegisterSystem<dbg::FrameBufferSystem>();
	m_ClientWorld.RegisterSystem<dbg::ImGuiDemoSystem>();
	m_ClientWorld.RegisterSystem<dbg::MenuBarSystem>();
	m_ClientWorld.RegisterSystem<dbg::NetworkSystem>();
	m_ClientWorld.RegisterSystem<dbg::NodeGraphSystem>();
	m_ClientWorld.RegisterSystem<dbg::OpenLevelSystem>();
	m_ClientWorld.RegisterSystem<dbg::OverlaySystem>();
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