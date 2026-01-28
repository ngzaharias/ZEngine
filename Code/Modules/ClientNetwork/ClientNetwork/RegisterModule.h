#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace client::network
{
	void RegisterModule(ecs::EntityWorld& entityWorld);
}