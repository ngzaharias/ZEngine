#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace sfml
{
	class Window;
}

namespace eng
{
	struct ClientDependencies
	{
		sfml::Window& m_Window;
	};

	struct ServerDependencies
	{
	};

	struct SharedDependencies
	{
	};

	void RegisterClientSystems(ecs::EntityWorld& entityWorld, const eng::ClientDependencies& dependencies);
	void RegisterServerSystems(ecs::EntityWorld& entityWorld, const eng::ServerDependencies& dependencies);
	void RegisterSharedSystems(ecs::EntityWorld& entityWorld, const eng::SharedDependencies& dependencies);
}