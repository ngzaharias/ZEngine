#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	void RegisterClientComponents(ecs::EntityWorld& entityWorld);
	void RegisterServerComponents(ecs::EntityWorld& entityWorld);
	void RegisterSharedComponents(ecs::EntityWorld& entityWorld);
}