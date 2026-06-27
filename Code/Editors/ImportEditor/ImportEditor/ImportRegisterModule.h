#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace editor::importer
{
	void RegisterModule(ecs::EntityWorld& world);
}