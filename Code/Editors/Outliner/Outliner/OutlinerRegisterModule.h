#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace editor::outliner
{
	void RegisterModule(ecs::EntityWorld& world);
}