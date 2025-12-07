#include "GameClientPCH.h"
#include "GameClient/GameClient.h"

#include "Container/RegisterModule.h"
#include "ECS/WorldView.h"
#include "Engine/AchievementTable.h"
#include "Engine/AssetManager.h"
#include "Engine/ColourTable.h"
#include "Engine/ImguiManager.h"
#include "Engine/NetworkManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PlatformManager.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RegisterComponents.h"
#include "Engine/RegisterSystems.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/ThemeTable.h"
#include "Engine/UIManager.h"
#include "Engine/WindowManager.h"
#include "GameClient/RegisterComponents.h"
#include "GameClient/RegisterSystems.h"
#include "GameShared/RegisterComponents.h"
#include "GameUI/RegisterModule.h"
#include "Hexmap/RegisterModule.h"
#include "Hidden/RegisterModule.h"
#include "Softbody/RegisterModule.h"
#include "Tabletop/RegisterModule.h"
#include "Tactics/RegisterModule.h"
#include "Tilemap/RegisterModule.h"
#include "Voxel/RegisterModule.h"

namespace
{
	const str::Name strNoesis = NAME("Noesis");
}

clt::GameClient::GameClient()
	: m_InputManager()
	, m_UIManager(m_EntityWorld)
	, m_ReplicationPeer(m_EntityWorld)
{
}

void clt::GameClient::Register(const Dependencies& dependencies)
{
	// resources
	{
		m_EntityWorld.RegisterResource(dependencies.m_AssetManager);
		m_EntityWorld.RegisterResource(dependencies.m_ImguiManager);
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager);
		m_EntityWorld.RegisterResource(dependencies.m_PhysicsManager);
		m_EntityWorld.RegisterResource(dependencies.m_PlatformManager);
		m_EntityWorld.RegisterResource(dependencies.m_PrototypeManager);
		m_EntityWorld.RegisterResource(dependencies.m_TableHeadmaster);
		m_EntityWorld.RegisterResource(dependencies.m_WindowManager);
		m_EntityWorld.RegisterResource(dependencies.m_Serializer);
		m_EntityWorld.RegisterResource(m_InputManager);
		m_EntityWorld.RegisterResource(m_UIManager);
		m_EntityWorld.RegisterResource(m_ReplicationPeer);

		// tables
		auto& headmaster = dependencies.m_TableHeadmaster;
		m_EntityWorld.RegisterResource(headmaster.GetManager<eng::AchievementTable>());
		m_EntityWorld.RegisterResource(headmaster.GetManager<eng::ColourTable>());
		m_EntityWorld.RegisterResource(headmaster.GetManager<eng::ThemeTable>());
	}

	// engine
	{
		eng::RegisterClientComponents(m_EntityWorld);
		eng::RegisterClientSystems(m_EntityWorld);

		eng::RegisterSharedComponents(m_EntityWorld, dependencies.m_Serializer);
		eng::RegisterSharedSystems(m_EntityWorld);
	}

	// shared
	{
		shd::RegisterComponents(m_EntityWorld, dependencies.m_Serializer);
	}

	// client
	{
		clt::RegisterComponents(m_EntityWorld);
		clt::RegisterSystems(m_EntityWorld);
	}

	// modules
	{
		container::RegisterModule(m_EntityWorld);
		gui::RegisterModule(m_EntityWorld);
		hexmap::RegisterModule(m_EntityWorld);
		hidden::RegisterModule(m_EntityWorld);
		softbody::RegisterModule(m_EntityWorld);
		tabletop::RegisterModule(m_EntityWorld);
		tactics::RegisterModule(m_EntityWorld);
		tilemap::RegisterModule(m_EntityWorld);
		voxel::RegisterModule(m_EntityWorld);
	}
}

void clt::GameClient::Initialise()
{
	m_EntityWorld.Initialise();

	// ui
	{
		const auto& windowManager = m_EntityWorld.ReadResource<eng::WindowManager>();
		m_UIManager.Initialise(*windowManager.GetWindow(0));

		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::GameUI;
		layer.m_Callback.Connect(m_UIManager, &eng::UIManager::ProcessInput);
		m_InputManager.AppendLayer(strNoesis, layer);
	}
}

void clt::GameClient::Shutdown()
{
	m_InputManager.RemoveLayer(strNoesis);
	m_UIManager.Shutdown();

	m_EntityWorld.Shutdown();
}

void clt::GameClient::PreUpdate(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto inputWorld = m_EntityWorld.GetWorldView<eng::InputManager::World>();
	m_InputManager.Update(inputWorld);
}

void clt::GameClient::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_EntityWorld.Update(gameTime);
	m_UIManager.Update(gameTime);
}

void clt::GameClient::PostUpdate(const GameTime& gameTime)
{
	PROFILE_FUNCTION();
}