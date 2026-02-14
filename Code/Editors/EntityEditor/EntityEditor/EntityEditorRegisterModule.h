#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace editor::entity
{
	void RegisterModule(ecs::EntityWorld& world);
}