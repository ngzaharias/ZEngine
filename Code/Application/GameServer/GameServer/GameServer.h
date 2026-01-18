#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/ReplicationHost.h"

class GameTime;

namespace ecs
{
	class TypeRegistry;
}

namespace eng
{
	class AssetManager;
	class PhysicsManager;
	class PrototypeManager;
}

namespace net
{
	class NetworkManager;
}

namespace server
{
	struct Dependencies
	{
		ecs::TypeRegistry& m_TypeRegistry;
		eng::AssetManager& m_AssetManager;
		eng::PhysicsManager& m_PhysicsManager;
		eng::PrototypeManager& m_PrototypeManager;
		net::NetworkManager& m_NetworkManager;
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
		ecs::ReplicationHost m_ReplicationHost;
	};
}