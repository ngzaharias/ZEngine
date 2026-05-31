#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::hexmap
{
	void RegisterModule(ecs::EntityWorld& world);
}