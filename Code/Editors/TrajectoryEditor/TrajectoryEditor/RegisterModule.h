#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace editor::trajectory
{
	void RegisterModule(ecs::EntityWorld& entityWorld);
}