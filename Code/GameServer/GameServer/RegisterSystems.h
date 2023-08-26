#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace glfw
{
	class Window;
}

namespace net
{
	class ReplicationHost;
}

namespace svr
{
	struct SystemDependencies
	{
		net::ReplicationHost& m_ReplicationHost;
	};

	void RegisterSystems(ecs::EntityWorld& entityWorld, const svr::SystemDependencies& dependencies);
}