#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace server::cursor
{
	void RegisterModule(ecs::EntityWorld& world);
}