#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/ReplicationPeer.h"
#include "Engine/InputManager.h"
#include "Engine/UIManager.h"

class GameTime;

namespace ecs
{
	class TypeRegistry;
}

namespace eng
{
	class AssetManager;
	class ImguiManager;
	class PhysicsManager;
	class PlatformManager;
	class PrototypeManager;
	class TableHeadmaster;
	class WindowManager;
}

namespace net
{
	class ComponentSerializer;
	class NetworkManager;
}

namespace client
{
	struct Dependencies
	{
		ecs::TypeRegistry& m_TypeRegistry;
		eng::AssetManager& m_AssetManager;
		eng::ImguiManager& m_ImguiManager;
		eng::PhysicsManager& m_PhysicsManager;
		eng::PlatformManager& m_PlatformManager;
		eng::PrototypeManager& m_PrototypeManager;
		eng::TableHeadmaster& m_TableHeadmaster;
		eng::WindowManager& m_WindowManager;
		net::NetworkManager& m_NetworkManager;
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
		ecs::ReplicationPeer m_ReplicationPeer;
		eng::InputManager m_InputManager;
		eng::UIManager m_UIManager;
	};
}