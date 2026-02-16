#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace settings
{
	void RegisterModule(ecs::EntityWorld& world);
}