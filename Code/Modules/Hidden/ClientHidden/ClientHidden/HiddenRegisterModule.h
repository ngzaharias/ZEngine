#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace client::hidden
{
	void RegisterModule(ecs::EntityWorld& world);
}