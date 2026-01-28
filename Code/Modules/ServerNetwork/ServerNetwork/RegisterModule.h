#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace server::network
{
	void RegisterModule(ecs::EntityWorld& entityWorld);
}