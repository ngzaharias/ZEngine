#include "GameServerPCH.h"
#include "GameServer/GameServer.h"

#include "Engine/AssetManager.h"
#include "Engine/NetworkManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RegisterComponents.h"
#include "Engine/RegisterSystems.h"
#include "GameServer/RegisterComponents.h"
#include "GameServer/RegisterSystems.h"
#include "GameShared/RegisterComponents.h"

svr::GameServer::GameServer()
	: m_ReplicationHost(m_EntityWorld)
{
}

void svr::GameServer::Register(const Dependencies& dependencies)
{
	// managers
	{
		m_EntityWorld.RegisterResource(dependencies.m_AssetManager);
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager);
		m_EntityWorld.RegisterResource(dependencies.m_PhysicsManager);
		m_EntityWorld.RegisterResource(dependencies.m_PrototypeManager);
		m_EntityWorld.RegisterResource(dependencies.m_Serializer);
		m_EntityWorld.RegisterResource(m_ReplicationHost);
	}

	// engine
	{
		eng::RegisterServerComponents(m_EntityWorld);
		eng::RegisterServerSystems(m_EntityWorld);

		eng::RegisterSharedComponents(m_EntityWorld, dependencies.m_Serializer);
		eng::RegisterSharedSystems(m_EntityWorld);
	}

	// shared
	{
		shd::RegisterComponents(m_EntityWorld, dependencies.m_Serializer);
	}

	// server
	{
		svr::RegisterComponents(m_EntityWorld);
		svr::RegisterSystems(m_EntityWorld);
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