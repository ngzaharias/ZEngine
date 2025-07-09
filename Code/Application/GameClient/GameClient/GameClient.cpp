#include "GameClientPCH.h"
#include "GameClient/GameClient.h"

#include "ECS/WorldView.h"
#include "Engine/AchievementTable.h"
#include "Engine/AssetManager.h"
#include "Engine/NetworkManager.h"
#include "Engine/NoesisManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PlatformManager.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RegisterComponents.h"
#include "Engine/RegisterSystems.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/WindowManager.h"
#include "GameClient/RegisterComponents.h"
#include "GameClient/RegisterSystems.h"
#include "GameShared/RegisterComponents.h"
#include "Hexmap/HexmapRootComponent.h"
#include "Hidden/HiddenCountComponent.h"
#include "Hidden/HiddenGroupComponent.h"
#include "Hidden/HiddenObjectComponent.h"
#include "Softbody/SoftbodyComponents.h"
#include "Tabletop/TabletopCardComponent.h"
#include "Tabletop/TabletopDeckComponent.h"

namespace
{
	const str::Name strNoesis = NAME("Noesis");
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
		prototypeManager.Register<hexmap::RootComponent>();
		prototypeManager.Register<hidden::CountComponent>();
		prototypeManager.Register<hidden::GroupComponent>();
		prototypeManager.Register<hidden::ObjectComponent>();
		prototypeManager.Register<softbody::ChainComponent>();
		prototypeManager.Register<tabletop::CardComponent>();
		prototypeManager.Register<tabletop::DeckComponent>();
	}

	// resources
	{
		m_EntityWorld.RegisterResource(dependencies.m_AssetManager);
		m_EntityWorld.RegisterResource(dependencies.m_NetworkManager);
		m_EntityWorld.RegisterResource(dependencies.m_PhysicsManager);
		m_EntityWorld.RegisterResource(dependencies.m_PlatformManager);
		m_EntityWorld.RegisterResource(dependencies.m_PrototypeManager);
		m_EntityWorld.RegisterResource(dependencies.m_TableHeadmaster);
		m_EntityWorld.RegisterResource(dependencies.m_WindowManager);
		m_EntityWorld.RegisterResource(dependencies.m_Serializer);
		m_EntityWorld.RegisterResource(dependencies.m_NoesisManager);
		m_EntityWorld.RegisterResource(m_InputManager);
		m_EntityWorld.RegisterResource(m_ReplicationPeer);

		// tables
		auto& headmaster = dependencies.m_TableHeadmaster;
		m_EntityWorld.RegisterResource(headmaster.GetManager<eng::AchievementTable>());
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

	{
		auto& noesisManager = m_EntityWorld.WriteResource<ui::NoesisManager>();

		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::GameUI;
		layer.m_Callback.Connect(noesisManager, &ui::NoesisManager::ProcessInput);
		m_InputManager.AppendLayer(strNoesis, layer);
	}
}

void clt::GameClient::Shutdown()
{
	m_InputManager.RemoveLayer(strNoesis);

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
}

void clt::GameClient::PostUpdate(const GameTime& gameTime)
{
	PROFILE_FUNCTION();
}