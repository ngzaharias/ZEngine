#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace server::hidden
{
	void RegisterModule(ecs::EntityWorld& entityWorld);
}