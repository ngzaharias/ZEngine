#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace debug::crafting
{
	void RegisterModule(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld);
}