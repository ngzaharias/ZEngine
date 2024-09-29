#include "GameClientPCH.h"
#include "GameClient/GameClient.h"

#include "Engine/AssetManager.h"
#include "Engine/NetworkManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RegisterComponents.h"
#include "Engine/RegisterSystems.h"
#include "Engine/Window.h"
#include "GameClient/HiddenCountComponent.h"
#include "GameClient/HiddenGroupComponent.h"
#include "GameClient/HiddenObjectComponent.h"
#include "GameClient/RegisterComponents.h"
#include "GameClient/RegisterSystems.h"
#include "GameShared/RegisterComponents.h"

clt::GameClient::GameClient()
	: m_ReplicationPeer(m_EntityWorld)
{
}

void clt::GameClient::Register(const Dependencies& dependencies)
{
	// prototypes
	{
		auto& prototypeManager = dependencies.m_PrototypeManager;
		prototypeManager.Register<hidden::CountComponent>();
		prototypeManager.Register<hidden::GroupComponent>();
		prototypeManager.Register<hidden::ObjectComponent>();
	}

	// resources
	{
		m_EntityWorld.RegisterResource(dependencies.m_Window);
		m_EntityWorld.RegisterResource(dependencies.m_AssetManager);
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager);
		m_EntityWorld.RegisterResource(dependencies.m_PhysicsManager);
		m_EntityWorld.RegisterResource(dependencies.m_PrototypeManager);
		m_EntityWorld.RegisterResource(dependencies.m_Serializer);
		m_EntityWorld.RegisterResource(m_ReplicationPeer);
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
}

void clt::GameClient::Initialise()
{
	m_EntityWorld.Initialise();
}

void clt::GameClient::Shutdown()
{
	m_EntityWorld.Shutdown();
}

void clt::GameClient::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_EntityWorld.Update(gameTime);
}