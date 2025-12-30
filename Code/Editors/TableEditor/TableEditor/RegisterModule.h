#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace editor::table
{
	void RegisterModule(ecs::EntityWorld& entityWorld);
}