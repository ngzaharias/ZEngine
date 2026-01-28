#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace client::cursor
{
	void RegisterModule(ecs::EntityWorld& entityWorld);
}