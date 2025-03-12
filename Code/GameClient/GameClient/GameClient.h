#pragma once

#include "ECS/EntityWorld.h"
#include "Engine/ReplicationPeer.h"

class GameTime;

namespace eng
{
	class AssetManager;
	class NetworkManager;
	class PhysicsManager;
	class PlatformManager;
	class PrototypeManager;
	class TableHeadmaster;
	class WindowManager;
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
		eng::PlatformManager& m_PlatformManager;
		eng::PrototypeManager& m_PrototypeManager;
		eng::TableHeadmaster& m_TableHeadmaster;
		eng::WindowManager& m_WindowManager;
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