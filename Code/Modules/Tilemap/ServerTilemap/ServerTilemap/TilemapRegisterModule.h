#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace server::tilemap
{
	void RegisterModule(ecs::EntityWorld& world);
}