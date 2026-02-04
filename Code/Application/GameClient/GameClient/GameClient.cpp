#include "GameClientPCH.h"
#include "GameClient/GameClient.h"

#include "Camera/RegisterModule.h"
#include "ClientCursor/RegisterModule.h"
#include "ClientHidden/RegisterModule.h"
#include "ClientNetwork/RegisterModule.h"
#include "Container/RegisterModule.h"
#include "ECS/TypeRegistry.h"
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
#include "GameUI/RegisterModule.h"
#include "Hexmap/RegisterModule.h"
#include "Render/RegisterModule.h"
#include "SharedCursor/RegisterModule.h"
#include "SharedHidden/RegisterModule.h"
#include "SharedNetwork/RegisterModule.h"
#include "Softbody/RegisterModule.h"
#include "Tabletop/RegisterModule.h"
#include "Tactics/RegisterModule.h"
#include "Tilemap/RegisterModule.h"
#include "Voxel/RegisterModule.h"

namespace
{
	const str::Name strNoesis = NAME("Noesis");
}

client::GameClient::GameClient(ecs::TypeRegistry& typeRegistry)
	: m_TypeRegistry(typeRegistry)
	, m_EntityWorld(typeRegistry)
	, m_InputManager()
	, m_UIManager(m_EntityWorld)
	, m_ReplicationPeer(m_EntityWorld)
{
}

void client::GameClient::Register(const Dependencies& dependencies)
{
	// resources
	{
		m_EntityWorld.RegisterResource(dependencies.m_AssetManager);
		m_EntityWorld.RegisterResource(dependencies.m_ImguiManager);
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager);
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager.GetFactory());
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager.GetHost());
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager.GetPeer());
		m_EntityWorld.RegisterResource(dependencies.m_PhysicsManager);
		m_EntityWorld.RegisterResource(dependencies.m_PlatformManager);
		m_EntityWorld.RegisterResource(dependencies.m_PrototypeManager);
		m_EntityWorld.RegisterResource(dependencies.m_TableHeadmaster);
		m_EntityWorld.RegisterResource(dependencies.m_WindowManager);
		m_EntityWorld.RegisterResource(m_InputManager);
		m_EntityWorld.RegisterResource(m_ReplicationPeer);
		m_EntityWorld.RegisterResource(m_TypeRegistry);
		m_EntityWorld.RegisterResource(m_UIManager);

		// tables
		auto& headmaster = dependencies.m_TableHeadmaster;
		m_EntityWorld.RegisterResource(headmaster.GetManager<eng::AchievementTable>());
		m_EntityWorld.RegisterResource(headmaster.GetManager<eng::ColourTable>());
		m_EntityWorld.RegisterResource(headmaster.GetManager<eng::ThemeTable>());
	}

	// engine
	{
		eng::RegisterSharedComponents(m_EntityWorld);
		eng::RegisterSharedSystems(m_EntityWorld);

		eng::RegisterClientComponents(m_EntityWorld);
		eng::RegisterClientSystems(m_EntityWorld);
	}

	// client
	{
		client::RegisterComponents(m_EntityWorld);
		client::RegisterSystems(m_EntityWorld);
	}

	// modules
	{
		camera::RegisterModule(m_EntityWorld);
		client::cursor::RegisterModule(m_EntityWorld);
		client::hidden::RegisterModule(m_EntityWorld);
		client::network::RegisterModule(m_EntityWorld);
		container::RegisterModule(m_EntityWorld);
		gui::RegisterModule(m_EntityWorld);
		hexmap::RegisterModule(m_EntityWorld);
		render::RegisterModule(m_EntityWorld);
		shared::cursor::RegisterModule(m_EntityWorld);
		shared::hidden::RegisterModule(m_EntityWorld);
		shared::network::RegisterModule(m_EntityWorld);
		softbody::RegisterModule(m_EntityWorld);
		tabletop::RegisterModule(m_EntityWorld);
		tactics::RegisterModule(m_EntityWorld);
		tilemap::RegisterModule(m_EntityWorld);
		voxel::RegisterModule(m_EntityWorld);
	}
}

void client::GameClient::Initialise()
{
	PROFILE_FUNCTION();

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

void client::GameClient::Shutdown()
{
	PROFILE_FUNCTION();

	m_InputManager.RemoveLayer(strNoesis);
	m_UIManager.Shutdown();

	m_EntityWorld.Shutdown();
}

void client::GameClient::PreUpdate(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto inputWorld = m_EntityWorld.WorldView<eng::InputManager::World>();
	m_InputManager.Update(inputWorld);
}

void client::GameClient::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_EntityWorld.Update(gameTime);
	m_UIManager.Update(gameTime);
}

void client::GameClient::PostUpdate(const GameTime& gameTime)
{
	PROFILE_FUNCTION();
}