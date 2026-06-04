#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace debug::tilemap
{
	void RegisterModule(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld);
}