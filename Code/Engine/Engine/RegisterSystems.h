#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	void RegisterClientSystems(ecs::EntityWorld& entityWorld);
	void RegisterServerSystems(ecs::EntityWorld& entityWorld);
	void RegisterSharedSystems(ecs::EntityWorld& entityWorld);
}