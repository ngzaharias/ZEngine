#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace editor::assets
{
	void RegisterModule(ecs::EntityWorld& world);
}