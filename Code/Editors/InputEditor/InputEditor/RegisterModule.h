#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace editor::input
{
	void RegisterModule(ecs::EntityWorld& entityWorld);
}