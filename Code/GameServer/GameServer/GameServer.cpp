#include "GameServerPCH.h"
#include "GameServer/GameServer.h"

#include <Engine/AssetManager.h>
#include <Engine/NetworkManager.h>
#include <Engine/PhysicsManager.h>
#include <Engine/PrototypeManager.h>
#include <Engine/RegisterComponents.h>
#include <Engine/RegisterSystems.h>

#include <GameShared/RegisterComponents.h>
#include <GameShared/RegisterSystems.h>

#include "GameServer/RegisterComponents.h"
#include "GameServer/RegisterSystems.h"

svr::GameServer::GameServer()
	: m_ReplicationHost(m_EntityWorld)
{
}

void svr::GameServer::Register(const Dependencies& dependencies)
{
	// managers
	{
		m_EntityWorld.AddResource(dependencies.m_AssetManager);
		m_EntityWorld.AddResource(dependencies.m_NetworkManager);
		m_EntityWorld.AddResource(dependencies.m_PhysicsManager);
		m_EntityWorld.AddResource(dependencies.m_PrototypeManager);
	}

	// engine
	{
		eng::RegisterServerComponents(m_EntityWorld);
		eng::RegisterSharedComponents(m_EntityWorld, dependencies.m_Serializer);

		eng::ServerDependencies serverDependencies = { };
		eng::SharedDependencies sharedDependencies = { };
		eng::RegisterServerSystems(m_EntityWorld, serverDependencies);
		eng::RegisterSharedSystems(m_EntityWorld, sharedDependencies);
	}

	// shared
	{
		shd::Dependencies sharedDependencies = { };
		shd::RegisterComponents(m_EntityWorld, dependencies.m_Serializer);
		shd::RegisterSystems(m_EntityWorld, sharedDependencies);
	}

	// server
	{
		svr::RegisterComponents(m_EntityWorld);

		svr::SystemDependencies serverDependencies = {
			m_ReplicationHost };
		svr::RegisterSystems(m_EntityWorld, serverDependencies);
	}
}

void svr::GameServer::Initialise()
{
	m_EntityWorld.Initialise();
}

void svr::GameServer::Shutdown()
{
	m_EntityWorld.Shutdown();
}

void svr::GameServer::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_EntityWorld.Update(gameTime);
}