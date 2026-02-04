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
	class NetworkManager;
	class PhysicsManager;
	class PlatformManager;
	class PrototypeManager;
	class TableHeadmaster;
	class WindowManager;
}

namespace client
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
	};

	class GameClient final
	{
	public:
		GameClient(ecs::TypeRegistry& typeRegistry);

		void Register(const Dependencies& dependencies);

		void Initialise();
		void Shutdown();

		void PreUpdate(const GameTime& gameTime);
		void Update(const GameTime& gameTime);
		void PostUpdate(const GameTime& gameTime);

	public:
		ecs::TypeRegistry& m_TypeRegistry;

		ecs::EntityWorld m_EntityWorld;
		ecs::ReplicationPeer m_ReplicationPeer;
		eng::InputManager m_InputManager;
		eng::UIManager m_UIManager;
	};
}