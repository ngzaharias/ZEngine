#include "GameClientPCH.h"
#include "GameClient/GameClient.h"

#include "Engine/AssetManager.h"
#include "Engine/NetworkManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RegisterComponents.h"
#include "Engine/RegisterSystems.h"
#include "Engine/GLFW/Window.h"
#include "GameClient/HiddenObjectPrototypes.h"
#include "GameClient/RegisterComponents.h"
#include "GameClient/RegisterSystems.h"
#include "GameShared/RegisterComponents.h"

namespace
{
	const str::Name strHiddenObject = NAME("HiddenObject");
	const str::Name strHiddenGroup = NAME("HiddenGroup");
}

clt::GameClient::GameClient()
	: m_ReplicationPeer(m_EntityWorld)
{
}

void clt::GameClient::Register(const Dependencies& dependencies)
{
	// prototypes
	{
		auto& prototypeManager = dependencies.m_PrototypeManager;
		prototypeManager.RegisterPrototype<hidden::ObjectPrototype, hidden::ObjectLoader>(strHiddenObject);
		prototypeManager.RegisterPrototype<hidden::GroupPrototype, hidden::GroupLoader>(strHiddenGroup);
	}

	// resources
	{
		m_EntityWorld.RegisterResource(dependencies.m_AssetManager);
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager);
		m_EntityWorld.RegisterResource(dependencies.m_PhysicsManager);
		m_EntityWorld.RegisterResource(dependencies.m_PrototypeManager);
		m_EntityWorld.RegisterResource(dependencies.m_Window);
	}

	// engine
	{
		eng::RegisterClientComponents(m_EntityWorld);
		eng::RegisterSharedComponents(m_EntityWorld, dependencies.m_Serializer);

		eng::ClientDependencies clientDependencies = { dependencies.m_Window };
		eng::SharedDependencies sharedDependencies = { };
		eng::RegisterClientSystems(m_EntityWorld, clientDependencies);
		eng::RegisterSharedSystems(m_EntityWorld, sharedDependencies);
	}

	// shared
	{
		shd::RegisterComponents(m_EntityWorld, dependencies.m_Serializer);
	}

	// client
	{
		clt::RegisterComponents(m_EntityWorld);

		clt::SystemDependencies clientDependencies = {
			dependencies.m_Window,
			m_ReplicationPeer,
			dependencies.m_Serializer };
		clt::RegisterSystems(m_EntityWorld, clientDependencies);
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