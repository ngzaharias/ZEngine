#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace server
{
	void RegisterComponents(ecs::EntityWorld& entityWorld);
}