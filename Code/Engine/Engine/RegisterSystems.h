#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace glfw
{
	class Window;
}

namespace eng
{
	struct ClientDependencies
	{
		glfw::Window& m_Window;
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