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
	class ComponentSerializer;
	class ReplicationPeer;
}

namespace clt
{
	struct SystemDependencies
	{
		glfw::Window& m_Window;
		net::ReplicationPeer& m_ReplicationPeer;
		net::ComponentSerializer& m_ComponentSerializer;
	};

	void RegisterSystems(ecs::EntityWorld& entityWorld, const clt::SystemDependencies& dependencies);
}