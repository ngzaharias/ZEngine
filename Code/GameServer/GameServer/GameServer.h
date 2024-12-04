#pragma once

#include "ECS/EntityWorld.h"
#include "Engine/ReplicationHost.h"

class GameTime;

namespace eng
{
	class AssetManager;
	class NetworkManager;
	class PhysicsManager;
	class PrototypeManager;
}

namespace net
{
	class ComponentSerializer;
}

namespace svr
{
	struct Dependencies
	{
		eng::AssetManager& m_AssetManager;
		eng::NetworkManager& m_NetworkManager;
		eng::PhysicsManager& m_PhysicsManager;
		eng::PrototypeManager& m_PrototypeManager;
		net::ComponentSerializer& m_Serializer;
	};

	class GameServer final
	{
	public:
		GameServer();

		void Register(const Dependencies& dependencies);

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

	public:
		ecs::EntityWorld m_EntityWorld;
		net::ReplicationHost m_ReplicationHost;
	};
}