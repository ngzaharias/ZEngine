#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace client
{
	void RegisterSystems(ecs::EntityWorld& entityWorld);
}