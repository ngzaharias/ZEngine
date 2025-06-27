#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace net
{
	class ComponentSerializer;
}

namespace eng
{
	void RegisterClientComponents(ecs::EntityWorld& entityWorld);
	void RegisterServerComponents(ecs::EntityWorld& entityWorld);
	void RegisterSharedComponents(ecs::EntityWorld& entityWorld, net::ComponentSerializer& serializer);
}