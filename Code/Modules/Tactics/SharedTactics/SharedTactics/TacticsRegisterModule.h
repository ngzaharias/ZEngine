#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::tactics
{
	void RegisterModule(ecs::EntityWorld& world);
}