#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace server::replication
{
	void RegisterModule(ecs::EntityWorld& world);
}