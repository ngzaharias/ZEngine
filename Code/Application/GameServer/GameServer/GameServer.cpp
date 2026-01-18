#include "GameServerPCH.h"
#include "GameServer/GameServer.h"

#include "ECS/TypeRegistry.h"
#include "Engine/AssetManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RegisterComponents.h"
#include "Engine/RegisterSystems.h"
#include "GameServer/RegisterComponents.h"
#include "GameServer/RegisterSystems.h"
#include "Network/NetworkManager.h"

server::GameServer::GameServer()
	: m_ReplicationHost(m_EntityWorld)
{
}

void server::GameServer::Register(const Dependencies& dependencies)
{
	// managers
	{
		m_EntityWorld.RegisterResource(dependencies.m_TypeRegistry);
		m_EntityWorld.RegisterResource(dependencies.m_AssetManager);
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager);
		m_EntityWorld.RegisterResource(dependencies.m_PhysicsManager);
		m_EntityWorld.RegisterResource(dependencies.m_PrototypeManager);
		m_EntityWorld.RegisterResource(m_ReplicationHost);
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