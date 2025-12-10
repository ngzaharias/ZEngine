#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace server
{
	void RegisterSystems(ecs::EntityWorld& entityWorld);
}