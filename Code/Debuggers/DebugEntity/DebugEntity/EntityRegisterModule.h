#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace debug::entity
{
	void RegisterModule(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld);
}