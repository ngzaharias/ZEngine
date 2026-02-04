#include "GameServerPCH.h"
#include "GameServer/GameServer.h"

#include "ECS/TypeRegistry.h"
#include "Engine/AssetManager.h"
#include "Engine/NetworkManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RegisterComponents.h"
#include "Engine/RegisterSystems.h"
#include "GameServer/RegisterComponents.h"
#include "GameServer/RegisterSystems.h"
#include "ServerCursor/CursorRegisterModule.h"
#include "ServerHidden/HiddenRegisterModule.h"
#include "ServerNetwork/NetworkRegisterModule.h"
#include "SharedCursor/CursorRegisterModule.h"
#include "SharedHidden/HiddenRegisterModule.h"
#include "SharedNetwork/NetworkRegisterModule.h"

server::GameServer::GameServer(ecs::TypeRegistry& typeRegistry)
	: m_TypeRegistry(typeRegistry)
	, m_EntityWorld(typeRegistry)
	, m_ReplicationHost(m_EntityWorld)
{
}

void server::GameServer::Register(const Dependencies& dependencies)
{
	// managers
	{
		m_EntityWorld.RegisterResource(dependencies.m_AssetManager);
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager);
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager.GetFactory());
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager.GetHost());
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager.GetPeer());
		m_EntityWorld.RegisterResource(dependencies.m_PhysicsManager);
		m_EntityWorld.RegisterResource(dependencies.m_PrototypeManager);
		m_EntityWorld.RegisterResource(m_ReplicationHost);
		m_EntityWorld.RegisterResource(m_TypeRegistry);
	}

	// engine
	{
		eng::RegisterServerComponents(m_EntityWorld);
		eng::RegisterServerSystems(m_EntityWorld);

		eng::RegisterSharedComponents(m_EntityWorld);
		eng::RegisterSharedSystems(m_EntityWorld);
	}

	// server
	{
		server::RegisterComponents(m_EntityWorld);
		server::RegisterSystems(m_EntityWorld);
	}

	// modules
	{
		server::cursor::RegisterModule(m_EntityWorld);
		server::hidden::RegisterModule(m_EntityWorld);
		server::network::RegisterModule(m_EntityWorld);
		shared::cursor::RegisterModule(m_EntityWorld);
		shared::hidden::RegisterModule(m_EntityWorld);
		shared::network::RegisterModule(m_EntityWorld);
	}
}

void server::GameServer::Initialise()
{
	PROFILE_FUNCTION();

	m_EntityWorld.Initialise();
}

void server::GameServer::Shutdown()
{
	PROFILE_FUNCTION();

	m_EntityWorld.Shutdown();
}

void server::GameServer::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_EntityWorld.Update(gameTime);
}