#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace editor::inspector
{
	void RegisterModule(ecs::EntityWorld& world);
}