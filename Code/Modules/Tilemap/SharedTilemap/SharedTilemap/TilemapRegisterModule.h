#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::tilemap
{
	void RegisterModule(ecs::EntityWorld& world);
}