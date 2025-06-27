#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace net
{
	class ComponentSerializer;
}

namespace shd
{
	void RegisterComponents(ecs::EntityWorld& entityWorld, net::ComponentSerializer& serializer);
}