#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::tabletop
{
	void RegisterModule(ecs::EntityWorld& world);
}