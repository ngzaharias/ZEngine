#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace svr
{
	void RegisterSystems(ecs::EntityWorld& entityWorld);
}