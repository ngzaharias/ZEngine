#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace gui
{
	void RegisterModule(ecs::EntityWorld& entityWorld);
}