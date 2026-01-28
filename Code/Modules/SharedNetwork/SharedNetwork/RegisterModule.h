#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::network
{
	void RegisterModule(ecs::EntityWorld& entityWorld);
}