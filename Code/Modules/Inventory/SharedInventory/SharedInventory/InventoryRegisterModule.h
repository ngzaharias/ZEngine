#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::inventory
{
	void RegisterModule(ecs::EntityWorld& world);
}