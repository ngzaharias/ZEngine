#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace render
{
	void RegisterModule(ecs::EntityWorld& world);
}