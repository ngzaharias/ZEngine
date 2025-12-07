#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace container
{
	void RegisterModule(ecs::EntityWorld& entityWorld);
}