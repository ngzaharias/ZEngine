#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::gamestate
{
	void RegisterModule(ecs::EntityWorld& world);
}