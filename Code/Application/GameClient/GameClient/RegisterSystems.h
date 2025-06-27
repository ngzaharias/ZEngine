#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace clt
{
	void RegisterSystems(ecs::EntityWorld& entityWorld);
}