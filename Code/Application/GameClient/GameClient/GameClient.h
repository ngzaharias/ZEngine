#pragma once

#include "ECS/EntityWorld.h"
#include "Engine/InputManager.h"
#include "Engine/ReplicationPeer.h"

class GameTime;

namespace eng
{
	class AssetManager;
	class ImguiManager;
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

namespace ui
{
	class NoesisManager;
}

namespace clt
{
	struct Dependencies
	{
		eng::AssetManager& m_AssetManager;
		eng::ImguiManager& m_ImguiManager;
		eng::NetworkManager& m_NetworkManager;
		eng::PhysicsManager& m_PhysicsManager;
		eng::PlatformManager& m_PlatformManager;
		eng::PrototypeManager& m_PrototypeManager;
		eng::TableHeadmaster& m_TableHeadmaster;
		eng::WindowManager& m_WindowManager;
		net::ComponentSerializer& m_Serializer;
		ui::NoesisManager& m_NoesisManager;
	};

	class GameClient final
	{
	public:
		GameClient();

		void Register(const Dependencies& dependencies);

		void Initialise();
		void Shutdown();

		void PreUpdate(const GameTime& gameTime);
		void Update(const GameTime& gameTime);
		void PostUpdate(const GameTime& gameTime);

	public:
		ecs::EntityWorld m_EntityWorld;
		eng::InputManager m_InputManager;
		net::ReplicationPeer m_ReplicationPeer;
	};
}