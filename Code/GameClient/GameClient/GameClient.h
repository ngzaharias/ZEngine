#pragma once

#include "ECS/EntityWorld.h"
#include "Engine/ReplicationPeer.h"

class GameTime;

namespace eng
{
	class AssetManager;
	class NetworkManager;
	class PhysicsManager;
	class PrototypeManager;
}

namespace glfw
{
	class Window;
}

namespace net
{
	class ComponentSerializer;
}

namespace clt
{
	struct Dependencies
	{
		eng::AssetManager& m_AssetManager;
		eng::NetworkManager& m_NetworkManager;
		eng::PhysicsManager& m_PhysicsManager;
		eng::PrototypeManager& m_PrototypeManager;

		glfw::Window& m_Window;

		net::ComponentSerializer& m_Serializer;
	};

	class GameClient final
	{
	public:
		GameClient();

		void Register(const Dependencies& dependencies);

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

	public:
		ecs::EntityWorld m_EntityWorld;
		net::ReplicationPeer m_ReplicationPeer;
	};
}