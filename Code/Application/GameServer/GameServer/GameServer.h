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
	class NetworkManager;
	class PhysicsManager;
	class PrototypeManager;
}

namespace server
{
	struct Dependencies
	{
		eng::AssetManager& m_AssetManager;
		eng::NetworkManager& m_NetworkManager;
		eng::PhysicsManager& m_PhysicsManager;
		eng::PrototypeManager& m_PrototypeManager;
	};

	class GameServer final
	{
	public:
		GameServer(ecs::TypeRegistry& typeRegistry);

		void Register(const Dependencies& dependencies);

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

	public:
		ecs::TypeRegistry& m_TypeRegistry;

		ecs::EntityWorld m_EntityWorld;
		ecs::ReplicationHost m_ReplicationHost;
	};
}