#pragma once

#include "Engine/AssetManager.h"
#include "Engine/ComponentSerializer.h"
#include "Engine/NetworkManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PrototypeManager.h"
#include "Engine/ReplicationHost.h"
#include "Engine/ReplicationPeer.h"

class GameTime;

namespace sfml
{
	class Window;
}

namespace eng
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Execute(int argc, char* argv[]);

	protected:
		virtual void Register();

		virtual void Initialise();
		virtual void Shutdown();

		virtual void Update(const GameTime& gameTime);

	protected:
		eng::AssetManager m_AssetManager;
		eng::NetworkManager m_NetworkManager;
		eng::PhysicsManager m_PhysicsManager;
		eng::PrototypeManager m_PrototypeManager;

		net::ComponentSerializer m_ComponentSerializer;

		sfml::Window* m_Window;
	};
}