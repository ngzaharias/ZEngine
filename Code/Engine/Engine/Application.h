#pragma once

#include "Engine/AssetManager.h"
#include "Engine/ComponentSerializer.h"
#include "Engine/ImguiManager.h"
#include "Engine/NetworkManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PlatformManager.h"
#include "Engine/PrototypeManager.h"
#include "Engine/ReplicationHost.h"
#include "Engine/ReplicationPeer.h"
#include "Engine/Window.h"

class GameTime;

namespace eng
{
	class IWindow;
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

		virtual void PreUpdate(const GameTime& gameTime);
		virtual void Update(const GameTime& gameTime);
		virtual void PostUpdate(const GameTime& gameTime);

	protected:
		eng::Window* m_Window;
		eng::WindowConfig m_WindowConfig;

		eng::AssetManager m_AssetManager;
		eng::ImguiManager m_ImguiManager;
		eng::NetworkManager m_NetworkManager;
		eng::PhysicsManager m_PhysicsManager;
		eng::PlatformManager m_PlatformManager;
		eng::PrototypeManager m_PrototypeManager;
		net::ComponentSerializer m_ComponentSerializer;
	};
}