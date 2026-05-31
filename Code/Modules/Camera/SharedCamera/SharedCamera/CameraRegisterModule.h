#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::camera
{
	void RegisterModule(ecs::EntityWorld& world);
}