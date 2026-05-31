#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace gamestate
{
	void RegisterModule(ecs::EntityWorld& world);
}