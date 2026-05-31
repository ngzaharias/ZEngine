#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::hidden
{
	void RegisterModule(ecs::EntityWorld& world);
}