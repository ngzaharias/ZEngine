#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace server::inventory
{
	void RegisterModule(ecs::EntityWorld& world);
}