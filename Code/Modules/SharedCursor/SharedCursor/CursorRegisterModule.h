#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::cursor
{
	void RegisterModule(ecs::EntityWorld& world);
}