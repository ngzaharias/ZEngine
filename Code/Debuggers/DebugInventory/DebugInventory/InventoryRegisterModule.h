#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace debug::inventory
{
	void RegisterModule(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld);
}