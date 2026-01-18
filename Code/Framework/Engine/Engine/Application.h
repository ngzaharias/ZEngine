#pragma once

#include "ECS/TypeRegistry.h"
#include "Engine/AssetManager.h"
#include "Engine/ImguiManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PlatformManager.h"
#include "Engine/PrototypeManager.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/WindowManager.h"
#include "Network/NetworkManager.h"

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

		virtual bool ShouldClose();

	protected:
		ecs::TypeRegistry m_TypeRegistry;
		eng::AssetManager m_AssetManager;
		eng::ImguiManager m_ImguiManager;
		eng::PhysicsManager m_PhysicsManager;
		eng::PlatformManager m_PlatformManager;
		eng::PrototypeManager m_PrototypeManager;
		eng::TableHeadmaster m_TableHeadmaster;
		eng::WindowManager m_WindowManager;
		net::NetworkManager m_NetworkManager;
	};
}